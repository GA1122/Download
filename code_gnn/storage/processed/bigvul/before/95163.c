static void cmd_setacl(char *tag, const char *name,
                const char *identifier, const char *rights)
{
    int r;
    mbentry_t *mbentry = NULL;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

     
    r = mlookup(tag, name, intname, &mbentry);
    if (r == IMAP_MAILBOX_MOVED) return;

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
         
        struct backend *s = NULL;
        int res;

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        if (!s) r = IMAP_SERVER_UNAVAILABLE;

        if (!r && imapd_userisadmin && supports_referrals) {
             
            imapd_refer(tag, mbentry->server, name);
            referral_kick = 1;
            mboxlist_entry_free(&mbentry);
            return;
        }

        mboxlist_entry_free(&mbentry);

        if (!r) {
            if (rights) {
                prot_printf(s->out,
                            "%s Setacl {" SIZE_T_FMT "+}\r\n%s"
                            " {" SIZE_T_FMT "+}\r\n%s {" SIZE_T_FMT "+}\r\n%s\r\n",
                            tag, strlen(name), name,
                            strlen(identifier), identifier,
                            strlen(rights), rights);
            } else {
                prot_printf(s->out,
                            "%s Deleteacl {" SIZE_T_FMT "+}\r\n%s"
                            " {" SIZE_T_FMT "+}\r\n%s\r\n",
                            tag, strlen(name), name,
                            strlen(identifier), identifier);
            }
            res = pipe_until_tag(s, tag, 0);

            if (!CAPA(s, CAPA_MUPDATE) && res == PROXY_OK) {
                 
            }
             
            if (ultraparanoid && res == PROXY_OK) kick_mupdate();
        }

        imapd_check(s, 0);

        if (r) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        } else {
             
            prot_printf(imapd_out, "%s %s", tag, s->last_result.s);
        }

        free(intname);
        return;
    }

    mboxlist_entry_free(&mbentry);

     
    if (!r) {
        char *err;

         
        if (rights && *rights) {
            r = cyrus_acl_checkstr(rights, &err);
            if (r) {
                prot_printf(imapd_out, "%s BAD %s\r\n", tag, err);
                free(err);
                free(intname);
                return;
            }
        }

        r = mboxlist_setacl(&imapd_namespace, intname, identifier, rights,
                            imapd_userisadmin || imapd_userisproxyadmin,
                            proxy_userid, imapd_authstate);
    }

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        if (config_mupdate_server)
            kick_mupdate();

        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }
    free(intname);
}
