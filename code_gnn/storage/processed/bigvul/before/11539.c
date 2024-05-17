char* gethostname_malloc(void) {
        struct utsname u;

        assert_se(uname(&u) >= 0);

        if (u.nodename[0])
                return strdup(u.nodename);

        return strdup(u.sysname);
}
