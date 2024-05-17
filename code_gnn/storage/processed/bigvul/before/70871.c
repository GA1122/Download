static int parse_cookie(HTTPContext *s, const char *p, AVDictionary **cookies)
{
    char *eql, *name;

    if (!(eql = strchr(p, '='))) return AVERROR(EINVAL);
    if (!(name = av_strndup(p, eql - p))) return AVERROR(ENOMEM);

    av_dict_set(cookies, name, eql, AV_DICT_DONT_STRDUP_KEY);

    return 0;
}
