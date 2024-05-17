cib_peer_callback(xmlNode * msg, void *private_data)
{
    const char *reason = NULL;
    const char *originator = crm_element_value(msg, F_ORIG);

    if (originator == NULL || crm_str_eq(originator, cib_our_uname, TRUE)) {
         
        int bcast_id = 0;
        if (!(crm_element_value_int(msg, F_CIB_LOCAL_NOTIFY_ID, &bcast_id))) {
            check_local_notify(bcast_id);
        }
        return;

    } else if (crm_peer_cache == NULL) {
        reason = "membership not established";
        goto bail;
    }

    if (crm_element_value(msg, F_CIB_CLIENTNAME) == NULL) {
        crm_xml_add(msg, F_CIB_CLIENTNAME, originator);
    }

     
    cib_process_request(msg, FALSE, TRUE, TRUE, NULL);
    return;

  bail:
    if (reason) {
        const char *seq = crm_element_value(msg, F_SEQ);
        const char *op = crm_element_value(msg, F_CIB_OPERATION);

        crm_warn("Discarding %s message (%s) from %s: %s", op, seq, originator, reason);
    }
}