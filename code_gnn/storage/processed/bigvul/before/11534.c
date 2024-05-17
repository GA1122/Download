int get_process_cmdline(pid_t pid, size_t max_length, bool comm_fallback, char **line) {
        char *r, *k;
        int c;
        bool space = false;
        size_t left;
        FILE *f;

        assert(max_length > 0);
        assert(line);

        if (pid == 0)
                f = fopen("/proc/self/cmdline", "re");
        else {
                char *p;
                if (asprintf(&p, "/proc/%lu/cmdline", (unsigned long) pid) < 0)
                        return -ENOMEM;

                f = fopen(p, "re");
                free(p);
        }

        if (!f)
                return -errno;

        r = new(char, max_length);
        if (!r) {
                fclose(f);
                return -ENOMEM;
        }

        k = r;
        left = max_length;
        while ((c = getc(f)) != EOF) {

                if (isprint(c)) {
                        if (space) {
                                if (left <= 4)
                                        break;

                                *(k++) = ' ';
                                left--;
                                space = false;
                        }

                        if (left <= 4)
                                break;

                        *(k++) = (char) c;
                        left--;
                }  else
                        space = true;
        }

        if (left <= 4) {
                size_t n = MIN(left-1, 3U);
                memcpy(k, "...", n);
                k[n] = 0;
        } else
                *k = 0;

        fclose(f);

         
        if (r[0] == 0) {
                char *t;
                int h;

                free(r);

                if (!comm_fallback)
                        return -ENOENT;

                h = get_process_comm(pid, &t);
                if (h < 0)
                        return h;

                r = join("[", t, "]", NULL);
                free(t);

                if (!r)
                        return -ENOMEM;
        }

        *line = r;
        return 0;
}
