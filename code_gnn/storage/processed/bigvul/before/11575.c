int read_one_line_file(const char *fn, char **line) {
        FILE *f;
        int r;
        char t[LINE_MAX], *c;

        assert(fn);
        assert(line);

        f = fopen(fn, "re");
        if (!f)
                return -errno;

        if (!fgets(t, sizeof(t), f)) {

                if (ferror(f)) {
                        r = -errno;
                        goto finish;
                }

                t[0] = 0;
        }

        c = strdup(t);
        if (!c) {
                r = -ENOMEM;
                goto finish;
        }

        truncate_nl(c);

        *line = c;
        r = 0;

finish:
        fclose(f);
        return r;
}
