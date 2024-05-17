static void burl_append_encode_all (buffer * const b, const char * const str, const size_t len)
{
     
     
    char * const p = buffer_string_prepare_append(b, len*3);
    int j = 0;
    for (unsigned int i = 0; i < len; ++i, ++j) {
        if (burl_is_unreserved(str[i])) {
            p[j] = str[i];
        }
        else {
            p[j]   = '%';
            p[++j] = hex_chars_uc[(str[i] >> 4) & 0xF];
            p[++j] = hex_chars_uc[str[i] & 0xF];
        }
    }
    buffer_commit(b, j);
}
