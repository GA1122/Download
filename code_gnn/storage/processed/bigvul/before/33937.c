event_cb(smtp_session_t session, int event_no, void *arg, ...)
{
    int *ok;
    va_list alist;

    va_start(alist, arg);
    switch (event_no) {
        case SMTP_EV_CONNECT:
        case SMTP_EV_MAILSTATUS:
        case SMTP_EV_RCPTSTATUS:
        case SMTP_EV_MESSAGEDATA:
        case SMTP_EV_MESSAGESENT:
        case SMTP_EV_DISCONNECT:
            break;

        case SMTP_EV_WEAK_CIPHER:{
                int bits = va_arg(alist, long);
                ok = va_arg(alist, int *);

                crm_debug("SMTP_EV_WEAK_CIPHER, bits=%d - accepted.", bits);
                *ok = 1;
                break;
            }
        case SMTP_EV_STARTTLS_OK:
            crm_debug("SMTP_EV_STARTTLS_OK - TLS started here.");
            break;

        case SMTP_EV_INVALID_PEER_CERTIFICATE:{
                long vfy_result = va_arg(alist, long);
                ok = va_arg(alist, int *);

                 
                crm_err("SMTP_EV_INVALID_PEER_CERTIFICATE: %ld", vfy_result);
                *ok = 1;
                break;
            }
        case SMTP_EV_NO_PEER_CERTIFICATE:
            ok = va_arg(alist, int *);

            crm_debug("SMTP_EV_NO_PEER_CERTIFICATE - accepted.");
            *ok = 1;
            break;
        case SMTP_EV_WRONG_PEER_CERTIFICATE:
            ok = va_arg(alist, int *);

            crm_debug("SMTP_EV_WRONG_PEER_CERTIFICATE - accepted.");
            *ok = 1;
            break;
        case SMTP_EV_NO_CLIENT_CERTIFICATE:
            ok = va_arg(alist, int *);

            crm_debug("SMTP_EV_NO_CLIENT_CERTIFICATE - accepted.");
            *ok = 1;
            break;
        default:
            crm_debug("Got event: %d - ignored.\n", event_no);
    }
    va_end(alist);
}