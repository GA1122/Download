char *html_escape_string(const char *inp, char *dest,
                         const unsigned int len)
{
    int max;
    char *buf;
    unsigned char c;

    max = len * 6;

    if (dest == NULL && max)
        dest = malloc(sizeof (unsigned char) * (max + 1));

    if (dest == NULL)
        return NULL;

    buf = dest;
    while ((c = *inp++)) {
        switch (c) {
        case '>':
            *dest++ = '&';
            *dest++ = 'g';
            *dest++ = 't';
            *dest++ = ';';
            break;
        case '<':
            *dest++ = '&';
            *dest++ = 'l';
            *dest++ = 't';
            *dest++ = ';';
            break;
        case '&':
            *dest++ = '&';
            *dest++ = 'a';
            *dest++ = 'm';
            *dest++ = 'p';
            *dest++ = ';';
            break;
        case '\"':
            *dest++ = '&';
            *dest++ = 'q';
            *dest++ = 'u';
            *dest++ = 'o';
            *dest++ = 't';
            *dest++ = ';';
            break;
        default:
            *dest++ = c;
        }
    }
    *dest = '\0';
    return buf;
}
