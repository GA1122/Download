static int parse_annotate_fetch_data(const char *tag,
                                     int permessage_flag,
                                     strarray_t *entries,
                                     strarray_t *attribs)
{
    int c;
    static struct buf arg;

    c = prot_getc(imapd_in);
    if (c == EOF) {
        prot_printf(imapd_out,
                    "%s BAD Missing annotation entry\r\n", tag);
        goto baddata;
    }
    else if (c == '(') {
         
        do {
            if (permessage_flag)
                c = getastring(imapd_in, imapd_out, &arg);
            else
                c = getqstring(imapd_in, imapd_out, &arg);
            if (c == EOF) {
                prot_printf(imapd_out,
                            "%s BAD Missing annotation entry\r\n", tag);
                goto baddata;
            }

             
            strarray_append(entries, arg.s);

        } while (c == ' ');

        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close paren in annotation entry list \r\n",
                        tag);
            goto baddata;
        }

        c = prot_getc(imapd_in);
    }
    else {
         
        prot_ungetc(c, imapd_in);
        if (permessage_flag)
            c = getastring(imapd_in, imapd_out, &arg);
        else
            c = getqstring(imapd_in, imapd_out, &arg);
        if (c == EOF) {
            prot_printf(imapd_out,
                        "%s BAD Missing annotation entry\r\n", tag);
            goto baddata;
        }

        strarray_append(entries, arg.s);
    }

    if (c != ' ' || (c = prot_getc(imapd_in)) == EOF) {
        prot_printf(imapd_out,
                    "%s BAD Missing annotation attribute(s)\r\n", tag);
        goto baddata;
    }

    if (c == '(') {
         
        do {
            if (permessage_flag)
                c = getastring(imapd_in, imapd_out, &arg);
            else
                c = getqstring(imapd_in, imapd_out, &arg);
            if (c == EOF) {
                prot_printf(imapd_out,
                            "%s BAD Missing annotation attribute(s)\r\n", tag);
                goto baddata;
            }

             
            strarray_append(attribs, arg.s);

        } while (c == ' ');

        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close paren in "
                        "annotation attribute list\r\n", tag);
            goto baddata;
        }

        c = prot_getc(imapd_in);
    }
    else {
         
        prot_ungetc(c, imapd_in);
        if (permessage_flag)
            c = getastring(imapd_in, imapd_out, &arg);
        else
            c = getqstring(imapd_in, imapd_out, &arg);
        if (c == EOF) {
            prot_printf(imapd_out,
                        "%s BAD Missing annotation attribute\r\n", tag);
            goto baddata;
        }

        strarray_append(attribs, arg.s);
   }

    return c;

  baddata:
    if (c != EOF) prot_ungetc(c, imapd_in);
    return EOF;
}
