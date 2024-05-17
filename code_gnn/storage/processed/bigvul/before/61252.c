static int mboxlist_create_namecheck(const char *mboxname,
                                     const char *userid,
                                     const struct auth_state *auth_state,
                                     int isadmin, int force_subdirs)
{
    mbentry_t *mbentry = NULL;
    int r = 0;

     
    r = mboxname_policycheck(mboxname);
    if (r) goto done;

     
    if (!isadmin && mboxname_userownsmailbox(userid, mboxname)) {
         
        if (config_implicitrights & ACL_ADMIN)
            isadmin = 1;
    }

     
    r = mboxlist_lookup(mboxname, &mbentry, NULL);
    if (r != IMAP_MAILBOX_NONEXISTENT) {
        if (!r) {
            r = IMAP_MAILBOX_EXISTS;

             
            if (!isadmin &&
                !(cyrus_acl_myrights(auth_state, mbentry->acl) & ACL_LOOKUP)) {
                r = IMAP_PERMISSION_DENIED;
            }
        }

        goto done;
    }
    mboxlist_entry_free(&mbentry);

     
    r = mboxlist_findparent(mboxname, &mbentry);
    if (r == 0) {
         
        char root[MAX_MAILBOX_NAME+1];

         
        if (!isadmin &&
            !(cyrus_acl_myrights(auth_state, mbentry->acl) & ACL_CREATE)) {
            r = IMAP_PERMISSION_DENIED;
            goto done;
        }

         
        if (quota_findroot(root, sizeof(root), mboxname)) {
            quota_t qdiffs[QUOTA_NUMRESOURCES] = QUOTA_DIFFS_DONTCARE_INITIALIZER;
            qdiffs[QUOTA_NUMFOLDERS] = 1;
            r = quota_check_useds(root, qdiffs);
            if (r) goto done;
        }
    }
    else if (r == IMAP_MAILBOX_NONEXISTENT) {
         
        if (!isadmin) {
            r = IMAP_PERMISSION_DENIED;
            goto done;
        }

        if (!force_subdirs) {
            mbname_t *mbname = mbname_from_intname(mboxname);
            if (!mbname_isdeleted(mbname) && mbname_userid(mbname) && strarray_size(mbname_boxes(mbname))) {
                 
                r = IMAP_PERMISSION_DENIED;
                goto done;
            }
            mbname_free(&mbname);
        }

         
        r = 0;
    }

done:
    mboxlist_entry_free(&mbentry);

    return r;
}
