static int imapd_proxy_policy(sasl_conn_t *conn,
                              void *context,
                              const char *requested_user, unsigned rlen,
                              const char *auth_identity, unsigned alen,
                              const char *def_realm,
                              unsigned urlen,
                              struct propctx *propctx)
{
    char userbuf[MAX_MAILBOX_BUFFER];

    if (config_getswitch(IMAPOPT_IMAPMAGICPLUS)) {
        size_t n;
        char *p;

         
        if (!rlen) rlen = strlen(requested_user);
        if (rlen >= MAX_MAILBOX_BUFFER) {
            sasl_seterror(conn, 0, "buffer overflow while proxying");
            return SASL_BUFOVER;
        }
        memcpy(userbuf, requested_user, rlen);
        userbuf[rlen] = '\0';
        requested_user = userbuf;

         
        if ((p = strchr(userbuf, '+'))) {
            n = config_virtdomains ? strcspn(p, "@") : strlen(p);

             
            memmove(p, p+n, strlen(p+n)+1);
            rlen -= n;
        }
    }

    return mysasl_proxy_policy(conn, context, requested_user, rlen,
                               auth_identity, alen, def_realm, urlen, propctx);
}
