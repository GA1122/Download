int load_env_file(
                const char *fname,
                char ***rl) {

        FILE *f;
        char **m = NULL;
        int r;

        assert(fname);
        assert(rl);

        if (!(f = fopen(fname, "re")))
                return -errno;

        while (!feof(f)) {
                char l[LINE_MAX], *p, *u;
                char **t;

                if (!fgets(l, sizeof(l), f)) {
                        if (feof(f))
                                break;

                        r = -errno;
                        goto finish;
                }

                p = strstrip(l);

                if (!*p)
                        continue;

                if (strchr(COMMENTS, *p))
                        continue;

                if (!(u = normalize_env_assignment(p))) {
                        log_error("Out of memory");
                        r = -ENOMEM;
                        goto finish;
                }

                t = strv_append(m, u);
                free(u);

                if (!t) {
                        log_error("Out of memory");
                        r = -ENOMEM;
                        goto finish;
                }

                strv_free(m);
                m = t;
        }

        r = 0;

        *rl = m;
        m = NULL;

finish:
        if (f)
                fclose(f);

        strv_free(m);

        return r;
}
