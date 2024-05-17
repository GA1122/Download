EXPORTED int mboxlist_deletemailbox(const char *name, int isadmin,
                                    const char *userid,
                                    const struct auth_state *auth_state,
                                    struct mboxevent *mboxevent,
                                    int checkacl,
                                    int local_only, int force)
{
    mbentry_t *mbentry = NULL;
    int r = 0;
    long myrights;
    struct mailbox *mailbox = NULL;
    int isremote = 0;
    mupdate_handle *mupdate_h = NULL;

    if (!isadmin && force) return IMAP_PERMISSION_DENIED;

     
    mbname_t *mbname = mbname_from_intname(name);
    if (mbname_userid(mbname) && !strarray_size(mbname_boxes(mbname))) {
         
        if (!strcmpsafe(mbname_userid(mbname), userid)) {
            r = IMAP_MAILBOX_NOTSUPPORTED;
            goto done;
        }
         
        if (!isadmin) {
            r = IMAP_PERMISSION_DENIED;
            goto done;
        }
    }

    if (!isadmin && mbname_userid(mbname)) {
        struct buf attrib = BUF_INITIALIZER;
        annotatemore_lookup(mbname_intname(mbname), "/specialuse", mbname_userid(mbname), &attrib);
        if (attrib.len)
            r = IMAP_MAILBOX_SPECIALUSE;
        buf_free(&attrib);
        if (r) goto done;
    }

    r = mboxlist_lookup_allow_all(name, &mbentry, NULL);
    if (r) goto done;

    isremote = mbentry->mbtype & MBTYPE_REMOTE;

     
    if (checkacl) {
        myrights = cyrus_acl_myrights(auth_state, mbentry->acl);
        if(!(myrights & ACL_DELETEMBOX)) {
             
            if (mboxname_userownsmailbox(userid, name) &&
                (config_implicitrights & ACL_ADMIN)) {
                isadmin = 1;
            }

             
            r = (isadmin || (myrights & ACL_LOOKUP)) ?
                IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
            goto done;
        }
    }

     
    if (!isremote) {
        r = mailbox_open_iwl(name, &mailbox);
    }
    if (r && !force) goto done;

     
    if (!isremote && !local_only && config_mupdate_server) {
         
        r = mupdate_connect(config_mupdate_server, NULL, &mupdate_h, NULL);
        if (r) {
            syslog(LOG_ERR,
                   "cannot connect to mupdate server for delete of '%s'",
                   name);
            goto done;
        }
        r = mupdate_delete(mupdate_h, name);
        if(r) {
            syslog(LOG_ERR,
                   "MUPDATE: can't delete mailbox entry '%s'", name);
        }
        if (mupdate_h) mupdate_disconnect(&mupdate_h);
    }
    if (r && !force) goto done;

    if (!isremote && !mboxname_isdeletedmailbox(name, NULL)) {
         
        mbentry_t *newmbentry = mboxlist_entry_create();
        newmbentry->name = xstrdupnull(name);
        newmbentry->mbtype = MBTYPE_DELETED;
        if (mailbox) {
            newmbentry->uniqueid = xstrdupnull(mailbox->uniqueid);
            newmbentry->uidvalidity = mailbox->i.uidvalidity;
            newmbentry->foldermodseq = mailbox_modseq_dirty(mailbox);
        }
        r = mboxlist_update(newmbentry,  1);
        mboxlist_entry_free(&newmbentry);
    }
    else {
         
        r = mboxlist_update_entry(name, NULL, 0);
        if (r) {
            syslog(LOG_ERR, "DBERROR: error deleting %s: %s",
                   name, cyrusdb_strerror(r));
            r = IMAP_IOERROR;
            if (!force) goto done;
        }
        if (r && !force) goto done;
    }

     
    if (!isremote && mailbox) {
         
        sync_log_unmailbox(mailbox->name);
        mboxevent_extract_mailbox(mboxevent, mailbox);
        mboxevent_set_access(mboxevent, NULL, NULL, userid, mailbox->name, 1);

        r = mailbox_delete(&mailbox);
         
        if (r && mboxevent)
            mboxevent_free(&mboxevent);
    }

 done:
    mailbox_close(&mailbox);
    mboxlist_entry_free(&mbentry);
    mbname_free(&mbname);

    return r;
}
