static int get_cookies(HTTPContext *s, char **cookies, const char *path,
                       const char *domain)
{
    int ret = 0;
    char *next, *cookie, *set_cookies = av_strdup(s->cookies), *cset_cookies = set_cookies;

    if (!set_cookies) return AVERROR(EINVAL);

    av_dict_free(&s->cookie_dict);

    *cookies = NULL;
    while ((cookie = av_strtok(set_cookies, "\n", &next))) {
        int domain_offset = 0;
        char *param, *next_param, *cdomain = NULL, *cpath = NULL, *cvalue = NULL;
        set_cookies = NULL;

        if (parse_cookie(s, cookie, &s->cookie_dict))
            av_log(s, AV_LOG_WARNING, "Unable to parse '%s'\n", cookie);

        while ((param = av_strtok(cookie, "; ", &next_param))) {
            if (cookie) {
                cvalue = av_strdup(param);
                cookie = NULL;
            } else if (!av_strncasecmp("path=",   param, 5)) {
                av_free(cpath);
                cpath = av_strdup(&param[5]);
            } else if (!av_strncasecmp("domain=", param, 7)) {
                int leading_dot = (param[7] == '.');
                av_free(cdomain);
                cdomain = av_strdup(&param[7+leading_dot]);
            } else {
            }
        }
        if (!cdomain)
            cdomain = av_strdup(domain);

        if (!cdomain || !cpath || !cvalue) {
            av_log(s, AV_LOG_WARNING,
                   "Invalid cookie found, no value, path or domain specified\n");
            goto done_cookie;
        }

        if (av_strncasecmp(path, cpath, strlen(cpath)))
            goto done_cookie;

        domain_offset = strlen(domain) - strlen(cdomain);
        if (domain_offset < 0)
            goto done_cookie;

        if (av_strcasecmp(&domain[domain_offset], cdomain))
            goto done_cookie;

        if (!*cookies) {
            if (!(*cookies = av_strdup(cvalue))) {
                ret = AVERROR(ENOMEM);
                goto done_cookie;
            }
        } else {
            char *tmp = *cookies;
            size_t str_size = strlen(cvalue) + strlen(*cookies) + 3;
            if (!(*cookies = av_malloc(str_size))) {
                ret = AVERROR(ENOMEM);
                goto done_cookie;
            }
            snprintf(*cookies, str_size, "%s; %s", tmp, cvalue);
            av_free(tmp);
        }

        done_cookie:
        av_freep(&cdomain);
        av_freep(&cpath);
        av_freep(&cvalue);
        if (ret < 0) {
            if (*cookies) av_freep(cookies);
            av_free(cset_cookies);
            return ret;
        }
    }

    av_free(cset_cookies);

    return 0;
}
