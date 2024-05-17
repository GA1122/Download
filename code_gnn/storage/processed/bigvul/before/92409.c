 int resolve_dev_console(char **ret) {
        _cleanup_free_ char *active = NULL;
        char *tty;
        int r;

        assert(ret);

         

        if (path_is_read_only_fs("/sys") > 0)
                return -ENOMEDIUM;

        r = read_one_line_file("/sys/class/tty/console/active", &active);
        if (r < 0)
                return r;

         
        tty = strrchr(active, ' ');
        if (tty)
                tty++;
        else
                tty = active;

        if (streq(tty, "tty0")) {
                active = mfree(active);

                 
                r = read_one_line_file("/sys/class/tty/tty0/active", &active);
                if (r < 0)
                        return r;

                tty = active;
        }

        if (tty == active)
                *ret = TAKE_PTR(active);
        else {
                char *tmp;

                tmp = strdup(tty);
                if (!tmp)
                        return -ENOMEM;

                *ret = tmp;
        }

        return 0;
}
