static int parse_annotate_store_data(const char *tag,
                                     int permessage_flag,
                                     struct entryattlist **entryatts)
{
    int c, islist = 0;
    static struct buf entry, attrib, value;
    struct attvaluelist *attvalues = NULL;

    *entryatts = NULL;

    c = prot_getc(imapd_in);
    if (c == EOF) {
        prot_printf(imapd_out,
                    "%s BAD Missing annotation entry\r\n", tag);
        goto baddata;
    }
    else if (c == '(') {
         
        islist = 1;
    }
    else if (permessage_flag) {
        prot_printf(imapd_out,
                    "%s BAD Missing paren for annotation entry\r\n", tag);
        goto baddata;
    }
    else {
         
        prot_ungetc(c, imapd_in);
    }

    do {
         
        if (permessage_flag)
            c = getastring(imapd_in, imapd_out, &entry);
        else
            c = getqstring(imapd_in, imapd_out, &entry);
        if (c == EOF) {
            prot_printf(imapd_out,
                        "%s BAD Missing annotation entry\r\n", tag);
            goto baddata;
        }

         
        if (c != ' ' || (c = prot_getc(imapd_in)) != '(') {
            prot_printf(imapd_out,
                        "%s BAD Missing annotation attribute-values list\r\n",
                        tag);
            goto baddata;
        }

        do {
             
            if (permessage_flag)
                c = getastring(imapd_in, imapd_out, &attrib);
            else
                c = getqstring(imapd_in, imapd_out, &attrib);
            if (c == EOF) {
                prot_printf(imapd_out,
                            "%s BAD Missing annotation attribute\r\n", tag);
                goto baddata;
            }

             
            if (c != ' ') {
                prot_printf(imapd_out,
                            "%s BAD Missing annotation value\r\n", tag);
                goto baddata;
            }
            c = getbnstring(imapd_in, imapd_out, &value);
            if (c == EOF) {
                prot_printf(imapd_out,
                            "%s BAD Missing annotation value\r\n", tag);
                goto baddata;
            }

             
            appendattvalue(&attvalues, attrib.s, &value);

        } while (c == ' ');

        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close paren in annotation "
                        "attribute-values list\r\n", tag);
            goto baddata;
        }

         
        appendentryatt(entryatts, entry.s, attvalues);
        attvalues = NULL;

        c = prot_getc(imapd_in);

    } while (c == ' ');

    if (islist) {
        if (c != ')') {
            prot_printf(imapd_out,
                        "%s BAD Missing close paren in annotation entry list \r\n",
                        tag);
            goto baddata;
        }

        c = prot_getc(imapd_in);
    }

    return c;

  baddata:
    if (attvalues) freeattvalues(attvalues);
    if (c != EOF) prot_ungetc(c, imapd_in);
    return EOF;
}
