int getttyname_harder(int fd, char **r) {
        int k;
        char *s;

        if ((k = getttyname_malloc(fd, &s)) < 0)
                return k;

        if (streq(s, "tty")) {
                free(s);
                return get_ctty(0, NULL, r);
        }

        *r = s;
        return 0;
}
