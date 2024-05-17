scan_line_end(const char *s, int size)
{
    char *match;

    match = memchr(s, '\r', size - 1);
    if (!match) return 0;

     
    if (match[1] == '\n') return match - s + 2;

    return 0;
}
