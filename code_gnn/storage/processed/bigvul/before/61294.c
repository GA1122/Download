static int mboxlist_rmquota(const mbentry_t *mbentry, void *rock)
{
    int r = 0;
    struct mailbox *mailbox = NULL;
    const char *oldroot = (const char *) rock;

    assert(oldroot != NULL);

    r = mailbox_open_iwl(mbentry->name, &mailbox);
    if (r) goto done;

    if (mailbox->quotaroot) {
        if (strcmp(mailbox->quotaroot, oldroot)) {
             
            goto done;
        }

        r = mailbox_set_quotaroot(mailbox, NULL);
    }

 done:
    mailbox_close(&mailbox);

    if (r) {
        syslog(LOG_ERR, "LOSTQUOTA: unable to remove quota root %s for %s: %s",
               oldroot, mbentry->name, error_message(r));
    }

     
    return 0;
}
