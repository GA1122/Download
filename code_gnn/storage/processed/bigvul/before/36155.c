int mk_request_header_toc_parse(struct headers_toc *toc, const char *data, int len)
{
    int i = 0;
    int header_len;
    int colon;
    char *q;
    char *p = (char *) data;
    char *l = p;

    toc->length = 0;

    for (i = 0; l < (data + len) && p && i < MK_HEADERS_TOC_LEN; i++) {
        if (*p == '\r') goto out;

         
        colon = -1;
        for (q = p; *q != 0x0D; ++q) {
            if (*q == ':' && colon == -1) {
                colon = (q - p);
            }
        }

         
        if (*(q + 1) != 0x0A) {
            return -1;
        }

         
        if (data + len == (q - 1) && colon == -1) {
            break;
        }

         
        if (*(p + colon + 1) != 0x20) {
            return -1;
        }


         
        header_len = q - p - colon - 2;
        if (header_len == 0) {
            return -1;
        }

         
        toc->rows[i].init = p;
        toc->rows[i].end = q;
        toc->rows[i].status = 0;
        p = (q + mk_crlf.len);
        l = p;
        toc->length++;
    }

 out:
    return toc->length;
}
