int open_terminal(const char *name, int mode) {
        int fd, r;
        unsigned c = 0;

         

        for (;;) {
                if ((fd = open(name, mode)) >= 0)
                        break;

                if (errno != EIO)
                        return -errno;

                if (c >= 20)
                        return -errno;

                usleep(50 * USEC_PER_MSEC);
                c++;
        }

        if (fd < 0)
                return -errno;

        if ((r = isatty(fd)) < 0) {
                close_nointr_nofail(fd);
                return -errno;
        }

        if (!r) {
                close_nointr_nofail(fd);
                return -ENOTTY;
        }

        return fd;
}
