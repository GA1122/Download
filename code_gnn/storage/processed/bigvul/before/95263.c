static void specialuse_flags(const mbentry_t *mbentry, struct buf *attrib,
                             int isxlist)
{
    if (!mbentry) return;

    char *inbox = mboxname_user_mbox(imapd_userid, NULL);
    int inboxlen = strlen(inbox);

     
    if (strncmp(mbentry->name, inbox, inboxlen)) {
        free(inbox);
        return;
    }

     
    if (mbentry->name[inboxlen] == '\0') {
        if (isxlist) buf_init_ro_cstr(attrib, "\\Inbox");
    }
     
    else if (mbentry->name[inboxlen] == '.') {
         
        annotatemore_lookup(mbentry->name, "/specialuse", imapd_userid, attrib);
    }
    free(inbox);
     
}
