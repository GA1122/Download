mk_ptr_t mk_request_header_get(struct headers_toc *toc, const char *key_name, int key_len)
{
    int i;
    struct header_toc_row *row;
    mk_ptr_t var;

    var.data = NULL;
    var.len = 0;

    row = toc->rows;
    for (i = 0; i < toc->length; i++) {

         
        if (row[i].status == 1) {
            continue;
        }

        if (strncasecmp(row[i].init, key_name, key_len) == 0) {
            var.data = row[i].init + key_len + 1;
            var.len = row[i].end - var.data;
            row[i].status = 1;
            break;
        }
    }

    return var;
}