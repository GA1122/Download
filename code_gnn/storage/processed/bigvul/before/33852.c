cib_common_callback(qb_ipcs_connection_t *c, void *data, size_t size, gboolean privileged)
{
    uint32_t id = 0;
    uint32_t flags = 0;
    int call_options = 0;
    xmlNode *op_request = crm_ipcs_recv(c, data, size, &id, &flags);
    cib_client_t *cib_client = qb_ipcs_context_get(c);

    if(op_request) {
        crm_element_value_int(op_request, F_CIB_CALLOPTS, &call_options);
    }

    crm_trace("Inbound: %.200s", data);
    if (op_request == NULL || cib_client == NULL) {
        crm_ipcs_send_ack(c, id, "nack", __FUNCTION__, __LINE__);
        return 0;
    }

    if(is_set(call_options, cib_sync_call)) {
        CRM_ASSERT(flags & crm_ipc_client_response);
    }

    if(flags & crm_ipc_client_response) {
        CRM_LOG_ASSERT(cib_client->request_id == 0);  
        cib_client->request_id = id;                  
    }

    
    if (cib_client->name == NULL) {
        const char *value = crm_element_value(op_request, F_CIB_CLIENTNAME);
        if (value == NULL) {
            cib_client->name = crm_itoa(crm_ipcs_client_pid(c));
        } else {
            cib_client->name = strdup(value);
        }
    }

    if (cib_client->callback_id == NULL) {
        const char *value = crm_element_value(op_request, F_CIB_CALLBACK_TOKEN);
        if (value != NULL) {
            cib_client->callback_id = strdup(value);
            
        } else {
            cib_client->callback_id = strdup(cib_client->id);
        }
    }
    
    crm_xml_add(op_request, F_CIB_CLIENTID, cib_client->id);
    crm_xml_add(op_request, F_CIB_CLIENTNAME, cib_client->name);

#if ENABLE_ACL
    determine_request_user(cib_client->user, op_request, F_CIB_USER);
#endif

    crm_log_xml_trace(op_request, "Client[inbound]");

    cib_common_callback_worker(id, flags, op_request, cib_client, privileged);
    free_xml(op_request);

    return 0;
}