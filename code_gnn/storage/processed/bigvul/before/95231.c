static void list_response(const char *extname, const mbentry_t *mbentry,
                          uint32_t attributes, struct listargs *listargs)
{
    int r;
    struct statusdata sdata = STATUSDATA_INIT;
    struct buf specialuse = BUF_INITIALIZER;

    if ((attributes & MBOX_ATTRIBUTE_NONEXISTENT)) {
        if (!(listargs->cmd == LIST_CMD_EXTENDED)) {
            attributes |= MBOX_ATTRIBUTE_NOSELECT;
            attributes &= ~MBOX_ATTRIBUTE_NONEXISTENT;
        }
    }

    else if (listargs->scan) {
         

        if (!strcmpsafe(mbentry->name, index_mboxname(imapd_index))) {
             
            if (!index_scan(imapd_index, listargs->scan))
                return;  
        }
        else {
             
            struct index_state *state;
            struct index_init init;
            int doclose = 0;

            memset(&init, 0, sizeof(struct index_init));
            init.userid = imapd_userid;
            init.authstate = imapd_authstate;
            init.out = imapd_out;

            r = index_open(mbentry->name, &init, &state);

            if (!r)
                doclose = 1;

            if (!r && index_hasrights(state, ACL_READ)) {
                r = (imapd_userisadmin || index_hasrights(state, ACL_LOOKUP)) ?
                    IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
            }

            if (!r) {
                if (!index_scan(state, listargs->scan)) {
                    r = -1;   
                }
            }

            if (doclose) index_close(&state);

            if (r) return;
        }
    }

     
    uint32_t have_childinfo =
        MBOX_ATTRIBUTE_HASCHILDREN | MBOX_ATTRIBUTE_HASNOCHILDREN;
    if ((listargs->ret & LIST_RET_CHILDREN) && !(attributes & have_childinfo)) {
        if (imapd_namespace.isalt && !strcmp(extname, "INBOX")) {
             
        }
        else {
            char *intname = NULL, *freeme = NULL;

             
            if (mbentry)
                intname = mbentry->name;
            else
                intname = freeme = mboxname_from_external(extname, &imapd_namespace, imapd_userid);

            mboxlist_mboxtree(intname, set_haschildren, &attributes, MBOXTREE_SKIP_ROOT);
            if (freeme) free(freeme);
        }

        if (!(attributes & MBOX_ATTRIBUTE_HASCHILDREN))
            attributes |= MBOX_ATTRIBUTE_HASNOCHILDREN;
    }

    if (attributes & (MBOX_ATTRIBUTE_NONEXISTENT | MBOX_ATTRIBUTE_NOSELECT)) {
        int keep = 0;
         
        if (listargs->cmd == LIST_CMD_EXTENDED) {
            keep = 1;
        }
         
        if (listargs->cmd == LIST_CMD_LSUB) {
             
            if (attributes & MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED)
                keep = 1;
             
            if (config_mupdate_server)
                keep = 1;
        }
        else if (attributes & MBOX_ATTRIBUTE_HASCHILDREN)
            keep = 1;

        if (!keep) return;
    }

    if (listargs->cmd == LIST_CMD_LSUB) {
         
        if ( !(attributes & MBOX_ATTRIBUTE_SUBSCRIBED)
             && attributes & MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED)
            attributes |= MBOX_ATTRIBUTE_NOSELECT | MBOX_ATTRIBUTE_HASCHILDREN;
        attributes &= ~MBOX_ATTRIBUTE_SUBSCRIBED;
    }

     
    if (!(listargs->sel & LIST_SEL_RECURSIVEMATCH)) {
        attributes &= ~MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED;
    }

     
    if (attributes & MBOX_ATTRIBUTE_NOINFERIORS)
        attributes &= ~MBOX_ATTRIBUTE_HASCHILDREN;

     
    if (attributes & MBOX_ATTRIBUTE_HASCHILDREN)
        attributes &= ~MBOX_ATTRIBUTE_HASNOCHILDREN;

     
    if (listargs->cmd == LIST_CMD_EXTENDED) {
         
        if (attributes & MBOX_ATTRIBUTE_NOINFERIORS)
            attributes &= ~MBOX_ATTRIBUTE_HASNOCHILDREN;
         
        if (attributes & MBOX_ATTRIBUTE_NONEXISTENT)
            attributes &= ~MBOX_ATTRIBUTE_NOSELECT;
    }

    if (config_getswitch(IMAPOPT_SPECIALUSEALWAYS) ||
        listargs->cmd == LIST_CMD_XLIST ||
        listargs->ret & LIST_RET_SPECIALUSE) {
        specialuse_flags(mbentry, &specialuse, listargs->cmd == LIST_CMD_XLIST);
    }

    if (listargs->sel & LIST_SEL_SPECIALUSE) {
         
        if (!buf_len(&specialuse)) return;
    }

     
    if ((listargs->ret & LIST_RET_STATUS) && mbentry) {
        r = imapd_statusdata(mbentry->name, listargs->statusitems, &sdata);
        if (r) {
             
            attributes |= MBOX_ATTRIBUTE_NOSELECT;
        }
    }

    print_listresponse(listargs->cmd, extname,
                       imapd_namespace.hier_sep, attributes, &specialuse);
    buf_free(&specialuse);

    if ((listargs->ret & LIST_RET_STATUS) &&
        !(attributes & MBOX_ATTRIBUTE_NOSELECT)) {
         
        if (mbentry) print_statusline(extname, listargs->statusitems, &sdata);
    }

    if ((listargs->ret & LIST_RET_MYRIGHTS) &&
        !(attributes & MBOX_ATTRIBUTE_NOSELECT)) {
        if (mbentry) printmyrights(extname, mbentry);
    }

    if ((listargs->ret & LIST_RET_METADATA) &&
        !(attributes & MBOX_ATTRIBUTE_NOSELECT)) {
        if (mbentry)
            printmetadata(mbentry, &listargs->metaitems, &listargs->metaopts);
    }
}
