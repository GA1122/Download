char *http_escape_string(const char *inp, char *buf,
                         const unsigned int len)
{
    int max;
    char *index_c;
    unsigned char c;
    int found_a_colon = 0;

    max = len * 3;

    if (buf == NULL && max)
        buf = malloc(sizeof (unsigned char) * (max + 1));

    if (buf == NULL)
        return NULL;

    index_c = buf;
    while ((c = *inp++)) {
        if (c == ':' && !found_a_colon && index_c > buf) {
            found_a_colon = 1;
            memmove(buf + 2, buf, (index_c - buf));
            *buf = '.';
            *(buf + 1) = '/';
            index_c += 2;
            *index_c++ = ':';
        } else if (needs_escape((unsigned int) c) || c == '?') {
            *index_c++ = '%';
            *index_c++ = INT_TO_HEX((c >> 4) & 0xf);
            *index_c++ = INT_TO_HEX(c & 0xf);
        } else
            *index_c++ = c;
    }
    *index_c = '\0';

    return buf;
}
