parse_local_options(cib_client_t * cib_client, int call_type, int call_options, const char *host,
                    const char *op, gboolean * local_notify, gboolean * needs_reply,
                    gboolean * process, gboolean * needs_forward)
{
    if (cib_op_modifies(call_type)
        && !(call_options & cib_inhibit_bcast)) {
         
        *needs_reply = TRUE;
    } else {
        *needs_reply = FALSE;
    }

    if (host == NULL && (call_options & cib_scope_local)) {
        crm_trace("Processing locally scoped %s op from %s", op, cib_client->name);
        *local_notify = TRUE;

    } else if (host == NULL && cib_is_master) {
        crm_trace("Processing master %s op locally from %s", op, cib_client->name);
        *local_notify = TRUE;

    } else if (safe_str_eq(host, cib_our_uname)) {
        crm_trace("Processing locally addressed %s op from %s", op, cib_client->name);
        *local_notify = TRUE;

    } else if (stand_alone) {
        *needs_forward = FALSE;
        *local_notify = TRUE;
        *process = TRUE;

    } else {
        crm_trace("%s op from %s needs to be forwarded to %s",
                    op, cib_client->name, host ? host : "the master instance");
        *needs_forward = TRUE;
        *process = FALSE;
    }
}