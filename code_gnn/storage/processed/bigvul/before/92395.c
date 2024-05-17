int getttyname_malloc(int fd, char **ret) {
        char path[PATH_MAX], *c;  
        int r;

        assert(fd >= 0);
        assert(ret);

        r = ttyname_r(fd, path, sizeof path);  
        assert(r >= 0);
        if (r == ERANGE)
                return -ENAMETOOLONG;
        if (r > 0)
                return -r;

        c = strdup(skip_dev_prefix(path));
        if (!c)
                return -ENOMEM;

        *ret = c;
        return 0;
}
