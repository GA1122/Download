cib_process_request(xmlNode * request, gboolean force_synchronous, gboolean privileged,
                    gboolean from_peer, cib_client_t * cib_client)
{
    int call_type = 0;
    int call_options = 0;

    gboolean process = TRUE;
    gboolean is_update = TRUE;
    gboolean needs_reply = TRUE;
    gboolean local_notify = FALSE;
    gboolean needs_forward = FALSE;
    gboolean global_update = crm_is_true(crm_element_value(request, F_CIB_GLOBAL_UPDATE));

    xmlNode *op_reply = NULL;
    xmlNode *result_diff = NULL;

    int rc = pcmk_ok;
    const char *op = crm_element_value(request, F_CIB_OPERATION);
    const char *originator = crm_element_value(request, F_ORIG);
    const char *host = crm_element_value(request, F_CIB_HOST);
    const char *client_id = crm_element_value(request, F_CIB_CLIENTID);

    crm_trace("%s Processing msg %s", cib_our_uname, crm_element_value(request, F_SEQ));

    cib_num_ops++;
    if (cib_num_ops == 0) {
        cib_num_fail = 0;
        cib_num_local = 0;
        cib_num_updates = 0;
        crm_info("Stats wrapped around");
    }

    if (host != NULL && strlen(host) == 0) {
        host = NULL;
    }

    crm_element_value_int(request, F_CIB_CALLOPTS, &call_options);
    if (force_synchronous) {
        call_options |= cib_sync_call;
    }

    crm_trace("Processing %s message (%s) for %s...",
                from_peer ? "peer" : "local",
                from_peer ? originator : cib_our_uname, host ? host : "master");

    rc = cib_get_operation_id(op, &call_type);
    if (rc != pcmk_ok) {
         
        crm_err("Pre-processing of command failed: %s", pcmk_strerror(rc));
        return;
    }

    is_update = cib_op_modifies(call_type);
    if (is_update) {
        cib_num_updates++;
    }

    if (from_peer == FALSE) {
        parse_local_options(cib_client, call_type, call_options, host, op,
                            &local_notify, &needs_reply, &process, &needs_forward);

    } else if (parse_peer_options(call_type, request, &local_notify,
                                  &needs_reply, &process, &needs_forward) == FALSE) {
        return;
    }
    crm_trace("Finished determining processing actions");

    if (call_options & cib_discard_reply) {
        needs_reply = is_update;
        local_notify = FALSE;
    }

    if (needs_forward) {
        forward_request(request, cib_client, call_options);
        return;
    }

    if (cib_status != pcmk_ok) {
        rc = cib_status;
        crm_err("Operation ignored, cluster configuration is invalid."
                " Please repair and restart: %s", pcmk_strerror(cib_status));
        op_reply = cib_construct_reply(request, the_cib, cib_status);

    } else if (process) {
        int level = LOG_INFO;
        const char *section = crm_element_value(request, F_CIB_SECTION);

        cib_num_local++;
        rc = cib_process_command(request, &op_reply, &result_diff, privileged);

        if (global_update) {
            switch (rc) {
                case pcmk_ok:
                case -pcmk_err_old_data:
                case -pcmk_err_diff_resync:
                case -pcmk_err_diff_failed:
                    level = LOG_DEBUG_2;
                    break;
                default:
                    level = LOG_ERR;
            }

        } else if (safe_str_eq(op, CIB_OP_QUERY)) {
            level = LOG_DEBUG_2;

        } else if (rc != pcmk_ok) {
            cib_num_fail++;
            level = LOG_WARNING;

        } else if (safe_str_eq(op, CIB_OP_SLAVE)) {
            level = LOG_DEBUG_2;

        } else if (safe_str_eq(section, XML_CIB_TAG_STATUS)) {
            level = LOG_DEBUG_2;
        }

        do_crm_log_unlikely(level,
                       "Operation complete: op %s for section %s (origin=%s/%s/%s, version=%s.%s.%s): %s (rc=%d)",
                       op, section ? section : "'all'", originator ? originator : "local",
                       crm_element_value(request, F_CIB_CLIENTNAME), crm_element_value(request,
                                                                                       F_CIB_CALLID),
                       the_cib ? crm_element_value(the_cib, XML_ATTR_GENERATION_ADMIN) : "0",
                       the_cib ? crm_element_value(the_cib, XML_ATTR_GENERATION) : "0",
                       the_cib ? crm_element_value(the_cib, XML_ATTR_NUMUPDATES) : "0",
                       pcmk_strerror(rc), rc);

        if (op_reply == NULL && (needs_reply || local_notify)) {
            crm_err("Unexpected NULL reply to message");
            crm_log_xml_err(request, "null reply");
            needs_reply = FALSE;
            local_notify = FALSE;
        }
    }
    crm_trace("processing response cases %.16x %.16x", call_options, cib_sync_call);

     
    if (needs_reply == FALSE || stand_alone) {
         
        crm_trace("Completed slave update");

    } else if (rc == pcmk_ok && result_diff != NULL && !(call_options & cib_inhibit_bcast)) {
        gboolean broadcast = FALSE;

        cib_local_bcast_num++;
        crm_xml_add_int(request, F_CIB_LOCAL_NOTIFY_ID, cib_local_bcast_num);
        broadcast = send_peer_reply(request, result_diff, originator, TRUE);

        if (broadcast &&
            client_id &&
            local_notify &&
            op_reply) {

             
            local_notify = FALSE;
            queue_local_notify(op_reply, client_id, (call_options & cib_sync_call), from_peer);
            op_reply = NULL;  
        }

    } else if (call_options & cib_discard_reply) {
        crm_trace("Caller isn't interested in reply");

    } else if (from_peer) {
        if (is_update == FALSE || result_diff == NULL) {
            crm_trace("Request not broadcast: R/O call");

        } else if (call_options & cib_inhibit_bcast) {
            crm_trace("Request not broadcast: inhibited");

        } else if (rc != pcmk_ok) {
            crm_trace("Request not broadcast: call failed: %s", pcmk_strerror(rc));
        } else {
            crm_trace("Directing reply to %s", originator);
        }

        send_peer_reply(op_reply, result_diff, originator, FALSE);
    }

    if (local_notify && client_id) {
        if (process == FALSE) {
            do_local_notify(request, client_id, call_options & cib_sync_call, from_peer);
        } else {
            do_local_notify(op_reply, client_id, call_options & cib_sync_call, from_peer);
        }
    }

    free_xml(op_reply);
    free_xml(result_diff);

    return;
}