static int getliteralsize(const char *p, int c,
                          unsigned *size, int *binary, const char **parseerr)

{
    int isnowait = 0;
    uint32_t num;

     
    if (*p == '~') {
        p++;
        *binary = 1;
    }

     
    if (*p != '{') {
        *parseerr = "Missing required argument to Append command";
        return IMAP_PROTOCOL_ERROR;
    }

     
    if (parseuint32(p+1, &p, &num)) {
        *parseerr = "Literal size not a number";
        return IMAP_PROTOCOL_ERROR;
    }

    if (*p == '+') {
        isnowait++;
        p++;
    }

    if (c == '\r') {
        c = prot_getc(imapd_in);
    }

    if (*p != '}' || p[1] || c != '\n') {
        *parseerr = "Invalid literal in Append command";
        return IMAP_PROTOCOL_ERROR;
    }

    if (!isnowait) {
         
        prot_printf(imapd_out, "+ go ahead\r\n");
        prot_flush(imapd_out);
    }

    *size = num;

    return 0;
}