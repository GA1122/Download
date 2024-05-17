int getttyname_malloc(int fd, char **r) {
        char path[PATH_MAX], *c;
        int k;

        assert(r);

        if ((k = ttyname_r(fd, path, sizeof(path))) != 0)
                return -k;

        char_array_0(path);

        if (!(c = strdup(startswith(path, "/dev/") ? path + 5 : path)))
                return -ENOMEM;

        *r = c;
        return 0;
}
