static void cmd_rename(char *tag, char *oldname, char *newname, char *location)
{
    int r = 0;
    char *c;
    char oldmailboxname[MAX_MAILBOX_BUFFER];
    char newmailboxname[MAX_MAILBOX_BUFFER];
    char oldmailboxname2[MAX_MAILBOX_BUFFER];
    char newmailboxname2[MAX_MAILBOX_BUFFER];
    char *oldextname = NULL;
    char *newextname = NULL;
    char *oldintname = NULL;
    char *newintname = NULL;
    char *olduser = NULL;
    char *newuser = NULL;
    int omlen, nmlen;
    int subcount = 0;  
    int recursive_rename = 1;
    int rename_user = 0;
    mbentry_t *mbentry = NULL;
    struct renrock rock;

    if (location && !imapd_userisadmin) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_PERMISSION_DENIED));
        return;
    }

    if (location && strcmp(oldname, newname)) {
        prot_printf(imapd_out,
                    "%s NO Cross-server or cross-partition move w/rename not supported\r\n",
                    tag);
        return;
    }

    oldintname = mboxname_from_external(oldname, &imapd_namespace, imapd_userid);
    strncpy(oldmailboxname, oldintname, MAX_MAILBOX_NAME);
    free(oldintname);

    newintname = mboxname_from_external(newname, &imapd_namespace, imapd_userid);
    strncpy(newmailboxname, newintname, MAX_MAILBOX_NAME);
    free(newintname);

    olduser = mboxname_to_userid(oldmailboxname);
    newuser = mboxname_to_userid(newmailboxname);

     
    strcpy(oldmailboxname2, oldmailboxname);
    strcpy(newmailboxname2, newmailboxname);

    r = mlookup(NULL, NULL, oldmailboxname, &mbentry);

    if (!r && mbentry->mbtype & MBTYPE_REMOTE) {
         
        struct backend *s = NULL;
        int res;

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);

        if (!s) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_SERVER_UNAVAILABLE));
            goto done;
        }

        if (location) {
            char *destserver = NULL;
            char *destpart = NULL;

            c = strchr(location, '!');
            if (c) {
                destserver = xstrndup(location, c - location);
                destpart = xstrdup(c + 1);
            } else {
                destpart = xstrdup(location);
            }

            if (*destpart == '\0') {
                free(destpart);
                destpart = NULL;
            }

            if (!destserver || !strcmp(destserver, mbentry->server)) {
                 
                prot_printf(s->out,
                        "%s RENAME \"%s\" \"%s\" %s\r\n",
                        tag,
                        oldname,
                        newname,
                        location);
            } else {
                 
                prot_printf(s->out,
                        "%s XFER \"%s\" %s%s%s\r\n",
                        tag,
                        oldname,
                        destserver,
                        destpart ? " " : "",
                        destpart ? destpart : "");
            }

            if (destserver) free(destserver);
            if (destpart) free(destpart);

            res = pipe_until_tag(s, tag, 0);

             
            if (ultraparanoid && res == PROXY_OK) kick_mupdate();

        } else {  
            if (!strcmp(oldname, newname)) {
                prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(IMAP_SERVER_UNAVAILABLE));
                goto done;
            }

            prot_printf(s->out,
                    "%s RENAME \"%s\" \"%s\"\r\n",
                    tag,
                    oldname,
                    newname
                );

            res = pipe_until_tag(s, tag, 0);

             
            if (ultraparanoid && res == PROXY_OK) kick_mupdate();
        }

        imapd_check(s, 0);

        if (r) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        } else {
             
            prot_printf(imapd_out, "%s %s", tag, s->last_result.s);
        }

        goto done;
    }

     

    if (location && !config_partitiondir(location)) {
         
        char *server;
        char *partition;

         
        server = location;
        partition = strchr(location, '!');
        if (partition) *partition++ = '\0';

        cmd_xfer(tag, oldname, server, partition);

        goto done;
    }

     
    if (r == IMAP_MAILBOX_NONEXISTENT)
        r = 0;

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        goto done;
    }

     

     
    if (!strcasecmp(oldname, "inbox")) {
        recursive_rename = 0;
    }
     
    else if (config_getswitch(IMAPOPT_ALLOWUSERMOVES) &&
             mboxname_isusermailbox(oldmailboxname, 1) &&
             mboxname_isusermailbox(newmailboxname, 1) &&
             strcmp(oldmailboxname, newmailboxname) &&  
             imapd_userisadmin) {
        rename_user = 1;
    }

     
    omlen = strlen(oldmailboxname);
    nmlen = strlen(newmailboxname);
    if (omlen < nmlen) {
        if (!strncmp(oldmailboxname, newmailboxname, omlen) &&
            newmailboxname[omlen] == '.') {
            recursive_rename = 0;
        }
    } else {
        if (!strncmp(oldmailboxname, newmailboxname, nmlen) &&
            oldmailboxname[nmlen] == '.') {
            recursive_rename = 0;
        }
    }

    oldextname = mboxname_to_external(oldmailboxname, &imapd_namespace, imapd_userid);
    newextname = mboxname_to_external(newmailboxname, &imapd_namespace, imapd_userid);

     
    if (recursive_rename && strcmp(oldmailboxname, newmailboxname)) {
        int ol = omlen + 1;
        int nl = nmlen + 1;
        char ombn[MAX_MAILBOX_BUFFER];
        char nmbn[MAX_MAILBOX_BUFFER];

        strcpy(ombn, oldmailboxname);
        strcpy(nmbn, newmailboxname);
        strcat(ombn, ".");
        strcat(nmbn, ".");

         
        rock.namespace = &imapd_namespace;
        rock.found = 0;
        rock.newmailboxname = nmbn;
        rock.ol = ol;
        rock.nl = nl;
        rock.olduser = olduser;
        rock.newuser = newuser;
        rock.partition = location;
        rock.rename_user = rename_user;

         
        r = mboxlist_allmbox(ombn, checkmboxname, &rock, 0);

        subcount = rock.found;
    }

     
    if (!r) {
        struct mboxevent *mboxevent = NULL;
        uint32_t uidvalidity = mbentry ? mbentry->uidvalidity : 0;

         
        if (strcmp(oldmailboxname, newmailboxname))
            mboxevent = mboxevent_new(EVENT_MAILBOX_RENAME);

         
        mbentry_t *newmbentry = NULL;
        r = mboxlist_lookup_allow_all(newmailboxname, &newmbentry, NULL);
         
        if (!r && newmbentry->mbtype == MBTYPE_DELETED) {
             
            if (!mbentry || strcmpsafe(mbentry->uniqueid, newmbentry->uniqueid)) {
                 
                if (uidvalidity <= newmbentry->uidvalidity)
                    uidvalidity = newmbentry->uidvalidity+1;
            }
        }
        mboxlist_entry_free(&newmbentry);

        r = mboxlist_renamemailbox(oldmailboxname, newmailboxname, location,
                                   0  , imapd_userisadmin,
                                   imapd_userid, imapd_authstate, mboxevent,
                                   0, 0, rename_user);

         
        if (r == IMAP_MAILBOX_NONEXISTENT && subcount && !rename_user &&
           mboxname_userownsmailbox(imapd_userid, oldmailboxname) &&
           mboxname_userownsmailbox(imapd_userid, newmailboxname)) {

            mboxevent_free(&mboxevent);

            goto submboxes;
        }

         
        if (!r)
            mboxevent_notify(&mboxevent);
        mboxevent_free(&mboxevent);
    }

     
    if (!r && rename_user) {
        user_copyquotaroot(oldmailboxname, newmailboxname);
        user_renameacl(&imapd_namespace, newmailboxname, olduser, newuser);
        user_renamedata(olduser, newuser);

         
    }

     
    if (!r && recursive_rename) {
        prot_printf(imapd_out, "* OK rename %s %s\r\n",
                    oldextname, newextname);
        prot_flush(imapd_out);

submboxes:
        strcat(oldmailboxname, ".");
        strcat(newmailboxname, ".");

         
        rock.namespace = &imapd_namespace;
        rock.newmailboxname = newmailboxname;
        rock.ol = omlen + 1;
        rock.nl = nmlen + 1;
        rock.olduser = olduser;
        rock.newuser = newuser;
        rock.partition = location;
        rock.rename_user = rename_user;

         
        r = mboxlist_allmbox(oldmailboxname, renmbox, &rock, 0);
    }

     
    if (!r && rename_user) {
        user_deletedata(olduser, 1);
         
        sync_log_user(newuser);
         
        sync_log_unuser(olduser);
    }

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        if (config_mupdate_server)
            kick_mupdate();

        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
        if (rename_user) sync_log_user(newuser);
    }

done:
    mboxlist_entry_free(&mbentry);
    free(oldextname);
    free(newextname);
    free(olduser);
    free(newuser);
}
