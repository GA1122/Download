static inline int has_header(const char *str, const char *header)
{
     
    if (!str)
        return 0;
    return av_stristart(str, header + 2, NULL) || av_stristr(str, header);
}
