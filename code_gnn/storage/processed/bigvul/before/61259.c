mboxlist_delayed_deletemailbox(const char *name, int isadmin,
                               const char *userid,
                               const struct auth_state *auth_state,
                               struct mboxevent *mboxevent,
                               int checkacl,
                               int localonly,
                               int force)
{
    mbentry_t *mbentry = NULL;
    strarray_t existing = STRARRAY_INITIALIZER;
    int i;
    char newname[MAX_MAILBOX_BUFFER];
    int r = 0;
    long myrights;

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

    r = mboxlist_lookup(name, &mbentry, NULL);
    if (r) goto done;

     
    if (checkacl) {
        myrights = cyrus_acl_myrights(auth_state, mbentry->acl);
        if (!(myrights & ACL_DELETEMBOX)) {
             
            if (mboxname_userownsmailbox(userid, name) &&
                (config_implicitrights & ACL_ADMIN)) {
                isadmin = 1;
            }

             
            r = (isadmin || (myrights & ACL_LOOKUP)) ?
                IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;

            goto done;
        }
    }

     
    mboxname_todeleted(name, newname, 0);
    r = mboxlist_mboxtree(newname, addmbox_to_list, &existing, MBOXTREE_SKIP_ROOT);
    if (r) goto done;

     
    for (i = 0; i < (int)existing.count - 19; i++) {
        const char *subname = strarray_nth(&existing, i);
        syslog(LOG_NOTICE, "too many subfolders for %s, deleting %s (%d / %d)",
               newname, subname, i+1, (int)existing.count);
        r = mboxlist_deletemailbox(subname, 1, userid, auth_state, NULL, 0, 1, 1);
        if (r) goto done;
    }

     
    mboxname_todeleted(name, newname, 1);

     
    r = mboxlist_renamemailbox((char *)name, newname, mbentry->partition,
                               0  ,
                               1  , userid,
                               auth_state,
                               mboxevent,
                               localonly  ,
                               force, 1);

done:
    strarray_fini(&existing);
    mboxlist_entry_free(&mbentry);
    mbname_free(&mbname);

    return r;
}
