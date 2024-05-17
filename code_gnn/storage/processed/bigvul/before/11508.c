int close_nointr(int fd) {
        assert(fd >= 0);

        for (;;) {
                int r;

                r = close(fd);
                if (r >= 0)
                        return r;

                if (errno != EINTR)
                        return -errno;
        }
}
