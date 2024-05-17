static int trashacl(struct protstream *pin, struct protstream *pout,
                    char *mailbox)
{
    int i=0, j=0;
    char tagbuf[128];
    int c;               
    struct buf cmd, tmp, user;
    int r = 0;

    memset(&cmd, 0, sizeof(struct buf));
    memset(&tmp, 0, sizeof(struct buf));
    memset(&user, 0, sizeof(struct buf));

    prot_printf(pout, "ACL0 GETACL {" SIZE_T_FMT "+}\r\n%s\r\n",
                strlen(mailbox), mailbox);

    while(1) {
        c = prot_getc(pin);
        if (c != '*') {
            prot_ungetc(c, pin);
            r = getresult(pin, "ACL0");
            break;
        }

        c = prot_getc(pin);   
        c = getword(pin, &cmd);
        if (c == EOF) {
            r = IMAP_SERVER_UNAVAILABLE;
            break;
        }

        if (!strncmp(cmd.s, "ACL", 3)) {
            while(c != '\n') {
                 
                c = getastring(pin, pout, &tmp);
                if (c == EOF) {
                    r = IMAP_SERVER_UNAVAILABLE;
                    goto cleanup;
                }

                if(c == '\r') {
                    c = prot_getc(pin);
                    if(c != '\n') {
                        r = IMAP_SERVER_UNAVAILABLE;
                        goto cleanup;
                    }
                }
                if(c == '\n') break;   

                c = getastring(pin, pout, &user);
                if (c == EOF) {
                    r = IMAP_SERVER_UNAVAILABLE;
                    goto cleanup;
                }

                snprintf(tagbuf, sizeof(tagbuf), "ACL%d", ++i);

                prot_printf(pout, "%s DELETEACL {" SIZE_T_FMT "+}\r\n%s"
                            " {" SIZE_T_FMT "+}\r\n%s\r\n",
                            tagbuf, strlen(mailbox), mailbox,
                            strlen(user.s), user.s);
                if(c == '\r') {
                    c = prot_getc(pin);
                    if(c != '\n') {
                        r = IMAP_SERVER_UNAVAILABLE;
                        goto cleanup;
                    }
                }
                 
            }
        }
        else {
             
            eatline(pin, c);
        }
    }

    cleanup:

     
    if(!r) {
        while(j < i) {
            snprintf(tagbuf, sizeof(tagbuf), "ACL%d", ++j);
            r = getresult(pin, tagbuf);
            if (r) break;
        }
    }

    if(r) eatline(pin, c);

    buf_free(&user);
    buf_free(&tmp);
    buf_free(&cmd);

    return r;
}
