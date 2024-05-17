cib_pre_notify(int options, const char *op, xmlNode * existing, xmlNode * update)
{
    xmlNode *update_msg = NULL;
    const char *type = NULL;
    const char *id = NULL;
    gboolean needed = FALSE;

    g_hash_table_foreach(client_list, need_pre_notify, &needed);
    if (needed == FALSE) {
        return;
    }

     
    update_msg = create_xml_node(NULL, "pre-notify");

    if (update != NULL) {
        id = crm_element_value(update, XML_ATTR_ID);
    }

    crm_xml_add(update_msg, F_TYPE, T_CIB_NOTIFY);
    crm_xml_add(update_msg, F_SUBTYPE, T_CIB_PRE_NOTIFY);
    crm_xml_add(update_msg, F_CIB_OPERATION, op);

    if (id != NULL) {
        crm_xml_add(update_msg, F_CIB_OBJID, id);
    }

    if (update != NULL) {
        crm_xml_add(update_msg, F_CIB_OBJTYPE, crm_element_name(update));
    } else if (existing != NULL) {
        crm_xml_add(update_msg, F_CIB_OBJTYPE, crm_element_name(existing));
    }

    type = crm_element_value(update_msg, F_CIB_OBJTYPE);
    attach_cib_generation(update_msg, "cib_generation", the_cib);

    if (existing != NULL) {
        add_message_xml(update_msg, F_CIB_EXISTING, existing);
    }
    if (update != NULL) {
        add_message_xml(update_msg, F_CIB_UPDATE, update);
    }

    g_hash_table_foreach_remove(client_list, cib_notify_client, update_msg);

    if (update == NULL) {
        crm_trace("Performing operation %s (on section=%s)", op, type);

    } else {
        crm_trace("Performing %s on <%s%s%s>", op, type, id ? " id=" : "", id ? id : "");
    }

    free_xml(update_msg);
}