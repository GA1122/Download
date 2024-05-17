static void getlistargs(char *tag, struct listargs *listargs)
{
    static struct buf reference, buf;
    int c;

     
    c = prot_getc(imapd_in);
    if (c == '(') {
        listargs->cmd = LIST_CMD_EXTENDED;
        c = getlistselopts(tag, listargs);
        if (c == EOF) {
            eatline(imapd_in, c);
            return;
        }
    }
    else
        prot_ungetc(c, imapd_in);

    if (!strcmpsafe(imapd_magicplus, "+")) listargs->sel |= LIST_SEL_SUBSCRIBED;
    else if (!strcasecmpsafe(imapd_magicplus, "+dav")) listargs->sel |= LIST_SEL_DAV;

     
    c = getastring(imapd_in, imapd_out, &reference);
    if (c == EOF && !*reference.s) {
        prot_printf(imapd_out,
                    "%s BAD Missing required argument to List: reference name\r\n",
                    tag);
        eatline(imapd_in, c);
        return;
    }
    listargs->ref = reference.s;

    if (c != ' ') {
        prot_printf(imapd_out,
                    "%s BAD Missing required argument to List: mailbox pattern\r\n", tag);
        eatline(imapd_in, c);
        return;
    }

     
    c = prot_getc(imapd_in);
    if (c == '(') {
        listargs->cmd = LIST_CMD_EXTENDED;
        for (;;) {
            c = getastring(imapd_in, imapd_out, &buf);
            if (*buf.s)
                strarray_append(&listargs->pat, buf.s);
            if (c != ' ') break;
        }
        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Invalid syntax in List command\r\n", tag);
            eatline(imapd_in, c);
            goto freeargs;
        }
        c = prot_getc(imapd_in);
    }
    else {
        prot_ungetc(c, imapd_in);
        c = getastring(imapd_in, imapd_out, &buf);
        if (c == EOF) {
            prot_printf(imapd_out,
                        "%s BAD Missing required argument to List: mailbox pattern\r\n",
                        tag);
            eatline(imapd_in, c);
            goto freeargs;
        }
        strarray_append(&listargs->pat, buf.s);
    }

     
    if (c == ' ') {
        listargs->cmd = LIST_CMD_EXTENDED;
        c = getlistretopts(tag, listargs);
        if (c == EOF) {
            eatline(imapd_in, c);
            goto freeargs;
        }
    }

     
    if (c == '\r') c = prot_getc(imapd_in);
    if (c != '\n') {
        prot_printf(imapd_out,
                    "%s BAD Unexpected extra arguments to List\r\n", tag);
        eatline(imapd_in, c);
        goto freeargs;
    }

#ifdef USE_AUTOCREATE
    autocreate_inbox();
#endif  

    return;

  freeargs:
    strarray_fini(&listargs->pat);
    strarray_fini(&listargs->metaitems);
    return;
}
