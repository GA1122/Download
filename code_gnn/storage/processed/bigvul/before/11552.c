int make_null_stdio(void) {
        int null_fd;

        if ((null_fd = open("/dev/null", O_RDWR|O_NOCTTY)) < 0)
                return -errno;

        return make_stdio(null_fd);
}
