print_rsc_history(pe_working_set_t * data_set, node_t * node, xmlNode * rsc_entry)
{
    GListPtr gIter = NULL;
    GListPtr op_list = NULL;
    gboolean print_name = TRUE;
    GListPtr sorted_op_list = NULL;
    const char *rsc_id = crm_element_value(rsc_entry, XML_ATTR_ID);
    resource_t *rsc = pe_find_resource(data_set->resources, rsc_id);

    xmlNode *rsc_op = NULL;

    for (rsc_op = __xml_first_child(rsc_entry); rsc_op != NULL; rsc_op = __xml_next(rsc_op)) {
        if (crm_str_eq((const char *)rsc_op->name, XML_LRM_TAG_RSC_OP, TRUE)) {
            op_list = g_list_append(op_list, rsc_op);
        }
    }

    sorted_op_list = g_list_sort(op_list, sort_op_by_callid);
    for (gIter = sorted_op_list; gIter != NULL; gIter = gIter->next) {
        xmlNode *xml_op = (xmlNode *) gIter->data;
        const char *value = NULL;
        const char *call = crm_element_value(xml_op, XML_LRM_ATTR_CALLID);
        const char *task = crm_element_value(xml_op, XML_LRM_ATTR_TASK);
        const char *op_rc = crm_element_value(xml_op, XML_LRM_ATTR_RC);
        const char *interval = crm_element_value(xml_op, XML_LRM_ATTR_INTERVAL);
        int rc = crm_parse_int(op_rc, "0");

        if (safe_str_eq(task, CRMD_ACTION_STATUS)
            && safe_str_eq(interval, "0")) {
            task = "probe";
        }

        if (rc == 7 && safe_str_eq(task, "probe")) {
            continue;

        } else if (safe_str_eq(task, CRMD_ACTION_NOTIFY)) {
            continue;
        }

        if (print_name) {
            print_name = FALSE;
            if (rsc == NULL) {
                print_as("Orphan resource: %s", rsc_id);
            } else {
                print_rsc_summary(data_set, node, rsc, TRUE);
            }
        }

        print_as("    + (%s) %s:", call, task);
        if (safe_str_neq(interval, "0")) {
            print_as(" interval=%sms", interval);
        }

        if (print_timing) {
            int int_value;
            const char *attr = "last-rc-change";

            value = crm_element_value(xml_op, attr);
            if (value) {
                int_value = crm_parse_int(value, NULL);
                print_as(" %s=", attr);
                print_date(int_value);
            }

            attr = "last-run";
            value = crm_element_value(xml_op, attr);
            if (value) {
                int_value = crm_parse_int(value, NULL);
                print_as(" %s=", attr);
                print_date(int_value);
            }

            attr = "exec-time";
            value = crm_element_value(xml_op, attr);
            if (value) {
                int_value = crm_parse_int(value, NULL);
                print_as(" %s=%dms", attr, int_value);
            }

            attr = "queue-time";
            value = crm_element_value(xml_op, attr);
            if (value) {
                int_value = crm_parse_int(value, NULL);
                print_as(" %s=%dms", attr, int_value);
            }
        }

        print_as(" rc=%s (%s)\n", op_rc, lrmd_event_rc2str(rc));
    }

     
    g_list_free(sorted_op_list);
}