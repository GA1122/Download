static int list_data_remote(struct backend *be, char *tag,
                            struct listargs *listargs, strarray_t *subs)
{
    if ((listargs->cmd == LIST_CMD_EXTENDED) &&
        !CAPA(be, CAPA_LISTEXTENDED)) {
         
        prot_printf(imapd_out,
                    "%s NO Backend server does not support LIST-EXTENDED\r\n",
                    tag);
        return IMAP_MAILBOX_NOTSUPPORTED;
    }

     
    if (listargs->cmd == LIST_CMD_LSUB) {
        prot_printf(be->out, "%s Lsub ", tag);
    } else if (listargs->cmd == LIST_CMD_XLIST) {
        prot_printf(be->out, "%s Xlist ", tag);
    } else {
        prot_printf(be->out, "%s List ", tag);

        uint32_t select_mask = listargs->sel;

        if (be != backend_inbox) {
             
            select_mask &= ~(LIST_SEL_SUBSCRIBED | LIST_SEL_RECURSIVEMATCH);
        }

         
        if (select_mask) {
            const char *select_opts[] = {
                 
                "subscribed", "remote", "recursivematch",
                "special-use", "vendor.cmu-dav", "metadata", NULL
            };
            char c = '(';
            int i;

            for (i = 0; select_opts[i]; i++) {
                unsigned opt = (1 << i);

                if (!(select_mask & opt)) continue;

                prot_printf(be->out, "%c%s", c, select_opts[i]);
                c = ' ';

                if (opt == LIST_SEL_METADATA) {
                     
                    prot_puts(be->out, " (depth ");
                    if (listargs->metaopts.depth < 0) {
                        prot_puts(be->out, "infinity");
                    }
                    else {
                        prot_printf(be->out, "%d",
                                    listargs->metaopts.depth);
                    }
                    if (listargs->metaopts.maxsize) {
                        prot_printf(be->out, " maxsize %zu",
                                    listargs->metaopts.maxsize);
                    }
                    (void)prot_putc(')', be->out);
                }
            }
            prot_puts(be->out, ") ");
        }
    }

     
    prot_printf(be->out,
                "{%tu+}\r\n%s ", strlen(listargs->ref), listargs->ref);

     
    if (listargs->pat.count > 1) {
        char **p;
        char c = '(';

        for (p = listargs->pat.data ; *p ; p++) {
            prot_printf(be->out,
                        "%c{%tu+}\r\n%s", c, strlen(*p), *p);
            c = ' ';
        }
        (void)prot_putc(')', be->out);
    } else {
        prot_printf(be->out, "{%tu+}\r\n%s",
                    strlen(listargs->pat.data[0]), listargs->pat.data[0]);
    }

     
    if (listargs->ret && listargs->cmd == LIST_CMD_EXTENDED) {
        const char *return_opts[] = {
             
            "subscribed", "children", "special-use",
            "status ", "myrights", "metadata ", NULL
        };
        char c = '(';
        int i, j;

        prot_puts(be->out, " return ");
        for (i = 0; return_opts[i]; i++) {
            unsigned opt = (1 << i);

            if (!(listargs->ret & opt)) continue;

            prot_printf(be->out, "%c%s", c, return_opts[i]);
            c = ' ';

            if (opt == LIST_RET_STATUS) {
                 
                const char *status_items[] = {
                     
                    "messages", "recent", "uidnext", "uidvalidity", "unseen",
                    "highestmodseq", "xconvexists", "xconvunseen",
                    "xconvmodseq", NULL
                };

                c = '(';
                for (j = 0; status_items[j]; j++) {
                    if (!(listargs->statusitems & (1 << j))) continue;

                    prot_printf(be->out, "%c%s", c, status_items[j]);
                    c = ' ';
                }
                (void)prot_putc(')', be->out);
            }
            else if (opt == LIST_RET_METADATA) {
                 
                int n = strarray_size(&listargs->metaitems);

                c = '(';
                for (j = 0; j < n; j++) {
                    prot_printf(be->out, "%c\"%s\"", c,
                                strarray_nth(&listargs->metaitems, j));
                    c = ' ';
                }
                (void)prot_putc(')', be->out);
            }
        }
        (void)prot_putc(')', be->out);
    }

    prot_printf(be->out, "\r\n");
    pipe_lsub(be, imapd_userid, tag, 0, listargs, subs);

    return 0;
}
