int parse_env_file(
                const char *fname,
                const char *separator, ...) {

        int r = 0;
        char *contents = NULL, *p;

        assert(fname);
        assert(separator);

        if ((r = read_full_file(fname, &contents, NULL)) < 0)
                return r;

        p = contents;
        for (;;) {
                const char *key = NULL;

                p += strspn(p, separator);
                p += strspn(p, WHITESPACE);

                if (!*p)
                        break;

                if (!strchr(COMMENTS, *p)) {
                        va_list ap;
                        char **value;

                        va_start(ap, separator);
                        while ((key = va_arg(ap, char *))) {
                                size_t n;
                                char *v;

                                value = va_arg(ap, char **);

                                n = strlen(key);
                                if (strncmp(p, key, n) != 0 ||
                                    p[n] != '=')
                                        continue;

                                p += n + 1;
                                n = strcspn(p, separator);

                                if (n >= 2 &&
                                    strchr(QUOTES, p[0]) &&
                                    p[n-1] == p[0])
                                        v = strndup(p+1, n-2);
                                else
                                        v = strndup(p, n);

                                if (!v) {
                                        r = -ENOMEM;
                                        va_end(ap);
                                        goto fail;
                                }

                                if (v[0] == '\0') {
                                         
                                        free(v);
                                        v = NULL;
                                }

                                free(*value);
                                *value = v;

                                p += n;

                                r ++;
                                break;
                        }
                        va_end(ap);
                }

                if (!key)
                        p += strcspn(p, separator);
        }

fail:
        free(contents);
        return r;
}
