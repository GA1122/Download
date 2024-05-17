parse_peer_options(int call_type, xmlNode * request,
                   gboolean * local_notify, gboolean * needs_reply, gboolean * process,
                   gboolean * needs_forward)
{
    const char *op = NULL;
    const char *host = NULL;
    const char *delegated = NULL;
    const char *originator = crm_element_value(request, F_ORIG);
    const char *reply_to = crm_element_value(request, F_CIB_ISREPLY);
    const char *update = crm_element_value(request, F_CIB_GLOBAL_UPDATE);

    gboolean is_reply = safe_str_eq(reply_to, cib_our_uname);

    if (crm_is_true(update)) {
        *needs_reply = FALSE;
        if (is_reply) {
            *local_notify = TRUE;
            crm_trace("Processing global/peer update from %s"
                      " that originated from us", originator);
        } else {
            crm_trace("Processing global/peer update from %s", originator);
        }
        return TRUE;
    }

    host = crm_element_value(request, F_CIB_HOST);
    if (host != NULL && safe_str_eq(host, cib_our_uname)) {
        crm_trace("Processing request sent to us from %s", originator);
        return TRUE;

    } else if (host == NULL && cib_is_master == TRUE) {
        crm_trace("Processing request sent to master instance from %s", originator);
        return TRUE;
    }

    op = crm_element_value(request, F_CIB_OPERATION);
    if(safe_str_eq(op, "cib_shutdown_req")) {
         
        *local_notify = FALSE;
        if(reply_to == NULL || is_reply) {
            *process = TRUE;
        }
        if(is_reply) {
            *needs_reply = FALSE;
        }
        return *process;
    }

    if (is_reply) {
        crm_trace("Forward reply sent from %s to local clients", originator);
        *process = FALSE;
        *needs_reply = FALSE;
        *local_notify = TRUE;
        return TRUE;
    }

    delegated = crm_element_value(request, F_CIB_DELEGATED);
    if (delegated != NULL) {
        crm_trace("Ignoring msg for master instance");

    } else if (host != NULL) {
         
        crm_trace("Ignoring msg for instance on %s", crm_str(host));

    } else if (reply_to == NULL && cib_is_master == FALSE) {
         
        crm_trace("Ignoring reply to %s", crm_str(reply_to));

    } else if (safe_str_eq(op, "cib_shutdown_req")) {
        if (reply_to != NULL) {
            crm_debug("Processing %s from %s", op, host);
            *needs_reply = FALSE;

        } else {
            crm_debug("Processing %s reply from %s", op, host);
        }
        return TRUE;

    } else {
        crm_err("Nothing for us to do?");
        crm_log_xml_err(request, "Peer[inbound]");
    }

    return FALSE;
}