int get_ctty(pid_t pid, dev_t *ret_devnr, char **ret) {
        _cleanup_free_ char *fn = NULL, *b = NULL;
        dev_t devnr;
        int r;

        r = get_ctty_devnr(pid, &devnr);
        if (r < 0)
                return r;

        r = device_path_make_canonical(S_IFCHR, devnr, &fn);
        if (r < 0) {
                if (r != -ENOENT)  
                        return r;

                if (major(devnr) == 136) {
                         

                        if (asprintf(&b, "pts/%u", minor(devnr)) < 0)
                                return -ENOMEM;
                } else {
                         

                        r = device_path_make_major_minor(S_IFCHR, devnr, &fn);
                        if (r < 0)
                                return r;
                }
        }

        if (!b) {
                const char *w;

                w = path_startswith(fn, "/dev/");
                if (w) {
                        b = strdup(w);
                        if (!b)
                                return -ENOMEM;
                } else
                        b = TAKE_PTR(fn);
        }

        if (ret)
                *ret = TAKE_PTR(b);

        if (ret_devnr)
                *ret_devnr = devnr;

        return 0;
}
