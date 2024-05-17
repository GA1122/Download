static int imapd_canon_user(sasl_conn_t *conn, void *context,
                            const char *user, unsigned ulen,
                            unsigned flags, const char *user_realm,
                            char *out, unsigned out_max, unsigned *out_ulen)
{
    char userbuf[MAX_MAILBOX_BUFFER], *p;
    size_t n;
    int r;

    if (!ulen) ulen = strlen(user);

    if (config_getswitch(IMAPOPT_IMAPMAGICPLUS)) {
         
        if (ulen >= MAX_MAILBOX_BUFFER) {
            sasl_seterror(conn, 0, "buffer overflow while canonicalizing");
            return SASL_BUFOVER;
        }
        memcpy(userbuf, user, ulen);
        userbuf[ulen] = '\0';
        user = userbuf;

         
        if ((p = strchr(userbuf, '+'))) {
            n = config_virtdomains ? strcspn(p, "@") : strlen(p);

            if (flags & SASL_CU_AUTHZID) {
                 
                if (imapd_magicplus) free(imapd_magicplus);
                imapd_magicplus = xstrndup(p, n);
            }

             
            memmove(p, p+n, strlen(p+n)+1);
            ulen -= n;
        }
    }

    r = mysasl_canon_user(conn, context, user, ulen, flags, user_realm,
                          out, out_max, out_ulen);

    if (!r && imapd_magicplus && flags == SASL_CU_AUTHZID) {
         
        n = strlen(imapd_magicplus);
        if (*out_ulen + n > out_max) {
            sasl_seterror(conn, 0, "buffer overflow while canonicalizing");
            r = SASL_BUFOVER;
        }
        else {
            p = (config_virtdomains && (p = strchr(out, '@'))) ?
                p : out + *out_ulen;
            memmove(p+n, p, strlen(p)+1);
            memcpy(p, imapd_magicplus, n);
            *out_ulen += n;
        }
    }

    return r;
}
