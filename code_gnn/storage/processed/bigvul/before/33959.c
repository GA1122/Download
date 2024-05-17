send_smtp_trap(const char *node, const char *rsc, const char *task, int target_rc, int rc,
               int status, const char *desc)
{
#if ENABLE_ESMTP
    smtp_session_t session;
    smtp_message_t message;
    auth_context_t authctx;
    struct sigaction sa;

    int len = 20;
    int noauth = 1;
    int smtp_debug = LOG_DEBUG;
    char crm_mail_body[BODY_MAX];
    char *crm_mail_subject = NULL;

    memset(&sa, 0, sizeof(struct sigaction));

    if (node == NULL) {
        node = "-";
    }
    if (rsc == NULL) {
        rsc = "-";
    }
    if (desc == NULL) {
        desc = "-";
    }

    if (crm_mail_to == NULL) {
        return 1;
    }

    if (crm_mail_host == NULL) {
        crm_mail_host = "localhost:25";
    }

    if (crm_mail_prefix == NULL) {
        crm_mail_prefix = "Cluster notification";
    }

    crm_debug("Sending '%s' mail to %s via %s", crm_mail_prefix, crm_mail_to, crm_mail_host);

    len += strlen(crm_mail_prefix);
    len += strlen(task);
    len += strlen(rsc);
    len += strlen(node);
    len += strlen(desc);
    len++;

    crm_mail_subject = calloc(1, len);
    snprintf(crm_mail_subject, len, "%s - %s event for %s on %s: %s\r\n", crm_mail_prefix, task,
             rsc, node, desc);

    len = 0;
    len += snprintf(crm_mail_body + len, BODY_MAX - len, "\r\n%s\r\n", crm_mail_prefix);
    len += snprintf(crm_mail_body + len, BODY_MAX - len, "====\r\n\r\n");
    if (rc == target_rc) {
        len += snprintf(crm_mail_body + len, BODY_MAX - len,
                        "Completed operation %s for resource %s on %s\r\n", task, rsc, node);
    } else {
        len += snprintf(crm_mail_body + len, BODY_MAX - len,
                        "Operation %s for resource %s on %s failed: %s\r\n", task, rsc, node, desc);
    }

    len += snprintf(crm_mail_body + len, BODY_MAX - len, "\r\nDetails:\r\n");
    len += snprintf(crm_mail_body + len, BODY_MAX - len,
                    "\toperation status: (%d) %s\r\n", status, services_lrm_status_str(status));
    if (status == PCMK_LRM_OP_DONE) {
        len += snprintf(crm_mail_body + len, BODY_MAX - len,
                        "\tscript returned: (%d) %s\r\n", rc, lrmd_event_rc2str(rc));
        len += snprintf(crm_mail_body + len, BODY_MAX - len,
                        "\texpected return value: (%d) %s\r\n", target_rc,
                        lrmd_event_rc2str(target_rc));
    }

    auth_client_init();
    session = smtp_create_session();
    message = smtp_add_message(session);

    smtp_starttls_enable(session, Starttls_ENABLED);

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);

    smtp_set_server(session, crm_mail_host);

    authctx = auth_create_context();
    auth_set_mechanism_flags(authctx, AUTH_PLUGIN_PLAIN, 0);

    smtp_set_eventcb(session, event_cb, NULL);

     
    if (!noauth) {
        crm_debug("Adding authentication context");
        smtp_auth_set_context(session, authctx);
    }

    if (crm_mail_from == NULL) {
        struct utsname us;
        char auto_from[BODY_MAX];

        CRM_ASSERT(uname(&us) == 0);
        snprintf(auto_from, BODY_MAX, "crm_mon@%s", us.nodename);
        smtp_set_reverse_path(message, auto_from);

    } else {
         
        smtp_set_reverse_path(message, crm_mail_from);
    }

    smtp_set_header(message, "To", NULL   , NULL   );  
    smtp_add_recipient(message, crm_mail_to);

     
    smtp_set_header(message, "Subject", crm_mail_subject);
    smtp_set_header_option(message, "Subject", Hdr_OVERRIDE, 1);

    smtp_set_message_str(message, crm_mail_body);
    smtp_set_monitorcb(session, crm_smtp_debug, &smtp_debug, 1);

    if (smtp_start_session(session)) {
        char buf[128];
        int rc = smtp_errno();

        crm_err("SMTP server problem: %s (%d)", smtp_strerror(rc, buf, sizeof buf), rc);

    } else {
        char buf[128];
        int rc = smtp_errno();
        const smtp_status_t *smtp_status = smtp_message_transfer_status(message);

        if (rc != 0) {
            crm_err("SMTP server problem: %s (%d)", smtp_strerror(rc, buf, sizeof buf), rc);
        }
        crm_info("Send status: %d %s", smtp_status->code, crm_str(smtp_status->text));
        smtp_enumerate_recipients(message, print_recipient_status, NULL);
    }

    smtp_destroy_session(session);
    auth_destroy_context(authctx);
    auth_client_exit();
#endif
    return 0;
}