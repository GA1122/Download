int get_ctty(pid_t pid, dev_t *_devnr, char **r) {
        int k;
        char fn[PATH_MAX], *s, *b, *p;
        dev_t devnr;

        assert(r);

        k = get_ctty_devnr(pid, &devnr);
        if (k < 0)
                return k;

        snprintf(fn, sizeof(fn), "/dev/char/%u:%u", major(devnr), minor(devnr));
        char_array_0(fn);

        if ((k = readlink_malloc(fn, &s)) < 0) {

                if (k != -ENOENT)
                        return k;

                 
                if (major(devnr) == 136) {
                        if (asprintf(&b, "pts/%lu", (unsigned long) minor(devnr)) < 0)
                                return -ENOMEM;

                        *r = b;
                        if (_devnr)
                                *_devnr = devnr;

                        return 0;
                }

                 

                if (!(b = strdup(fn + 5)))
                        return -ENOMEM;

                *r = b;
                if (_devnr)
                        *_devnr = devnr;

                return 0;
        }

        if (startswith(s, "/dev/"))
                p = s + 5;
        else if (startswith(s, "../"))
                p = s + 3;
        else
                p = s;

        b = strdup(p);
        free(s);

        if (!b)
                return -ENOMEM;

        *r = b;
        if (_devnr)
                *_devnr = devnr;

        return 0;
}
