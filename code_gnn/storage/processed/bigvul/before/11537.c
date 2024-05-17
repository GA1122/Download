int get_process_uid(pid_t pid, uid_t *uid) {
        char *p;
        FILE *f;
        int r;

        assert(uid);

        if (pid == 0)
                return getuid();

        if (asprintf(&p, "/proc/%lu/status", (unsigned long) pid) < 0)
                return -ENOMEM;

        f = fopen(p, "re");
        free(p);

        if (!f)
                return -errno;

        while (!feof(f)) {
                char line[LINE_MAX], *l;

                if (!fgets(line, sizeof(line), f)) {
                        if (feof(f))
                                break;

                        r = -errno;
                        goto finish;
                }

                l = strstrip(line);

                if (startswith(l, "Uid:")) {
                        l += 4;
                        l += strspn(l, WHITESPACE);

                        l[strcspn(l, WHITESPACE)] = 0;

                        r = parse_uid(l, uid);
                        goto finish;
                }
        }

        r = -EIO;

finish:
        fclose(f);

        return r;
}
