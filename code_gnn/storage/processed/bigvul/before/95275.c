static int xfer_delete(struct xfer_header *xfer)
{
    mbentry_t *newentry = NULL;
    struct xfer_item *item;
    int r;

    syslog(LOG_INFO, "XFER: deleting mailboxes on source");

     
    for (item = xfer->items; item; item = item->next) {
         
         
        newentry = mboxlist_entry_create();
        newentry->name = xstrdupnull(item->mbentry->name);
        newentry->acl = xstrdupnull(item->mbentry->acl);
        newentry->server = xstrdupnull(item->mbentry->server);
        newentry->partition = xstrdupnull(item->mbentry->partition);
        newentry->mbtype = item->mbentry->mbtype|MBTYPE_DELETED;
        r = mboxlist_update(newentry, 1);
        mboxlist_entry_free(&newentry);

        if (r) {
            syslog(LOG_ERR,
                   "Could not move mailbox: %s, mboxlist_update failed (%s)",
                   item->mbentry->name, error_message(r));
        }

         
         
         
        r = mboxlist_deletemailbox(item->mbentry->name,
                                   imapd_userisadmin || imapd_userisproxyadmin,
                                   imapd_userid, imapd_authstate, NULL, 0, 1, 0);
        if (r) {
            syslog(LOG_ERR,
                   "Could not delete local mailbox during move of %s",
                   item->mbentry->name);
             
        }
    }

    return 0;
}
