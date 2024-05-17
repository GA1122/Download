int read_full_file(const char *fn, char **contents, size_t *size) {
        FILE *f;
        int r;
        size_t n, l;
        char *buf = NULL;
        struct stat st;

        if (!(f = fopen(fn, "re")))
                return -errno;

        if (fstat(fileno(f), &st) < 0) {
                r = -errno;
                goto finish;
        }

         
        if (st.st_size > 4*1024*1024) {
                r = -E2BIG;
                goto finish;
        }

        n = st.st_size > 0 ? st.st_size : LINE_MAX;
        l = 0;

        for (;;) {
                char *t;
                size_t k;

                if (!(t = realloc(buf, n+1))) {
                        r = -ENOMEM;
                        goto finish;
                }

                buf = t;
                k = fread(buf + l, 1, n - l, f);

                if (k <= 0) {
                        if (ferror(f)) {
                                r = -errno;
                                goto finish;
                        }

                        break;
                }

                l += k;
                n *= 2;

                 
                if (n > 4*1024*1024) {
                        r = -E2BIG;
                        goto finish;
                }
        }

        buf[l] = 0;
        *contents = buf;
        buf = NULL;

        if (size)
                *size = l;

        r = 0;

finish:
        fclose(f);
        free(buf);

        return r;
}
