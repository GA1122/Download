cib_common_callback_worker(uint32_t id, uint32_t flags, xmlNode * op_request, cib_client_t * cib_client, gboolean privileged)
{
    const char *op = crm_element_value(op_request, F_CIB_OPERATION);

    if (crm_str_eq(op, CRM_OP_REGISTER, TRUE)) {
        if(flags & crm_ipc_client_response) {
            xmlNode *ack = create_xml_node(NULL, __FUNCTION__);

            crm_xml_add(ack, F_CIB_OPERATION, CRM_OP_REGISTER);
            crm_xml_add(ack, F_CIB_CLIENTID, cib_client->id);
            crm_ipcs_send(cib_client->ipc, id, ack, FALSE);
            cib_client->request_id = 0;
            free_xml(ack);
        }
        return;

    } else if (crm_str_eq(op, T_CIB_NOTIFY, TRUE)) {
         
        int on_off = 0;
        const char *type = crm_element_value(op_request, F_CIB_NOTIFY_TYPE);
        crm_element_value_int(op_request, F_CIB_NOTIFY_ACTIVATE, &on_off);

        crm_debug("Setting %s callbacks for %s (%s): %s",
                  type, cib_client->name, cib_client->id, on_off ? "on" : "off");

        if (safe_str_eq(type, T_CIB_POST_NOTIFY)) {
            cib_client->post_notify = on_off;

        } else if (safe_str_eq(type, T_CIB_PRE_NOTIFY)) {
            cib_client->pre_notify = on_off;

        } else if (safe_str_eq(type, T_CIB_UPDATE_CONFIRM)) {
            cib_client->confirmations = on_off;

        } else if (safe_str_eq(type, T_CIB_DIFF_NOTIFY)) {
            cib_client->diffs = on_off;

        } else if (safe_str_eq(type, T_CIB_REPLACE_NOTIFY)) {
            cib_client->replace = on_off;
        }

        if(flags & crm_ipc_client_response) {
             
            crm_ipcs_send_ack(cib_client->ipc, id, "ack", __FUNCTION__, __LINE__);
            cib_client->request_id = 0;
        }
        return;
    }

    cib_client->num_calls++;
    cib_process_request(op_request, FALSE, privileged, FALSE, cib_client);
}