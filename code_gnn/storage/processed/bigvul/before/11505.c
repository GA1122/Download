int chvt(int vt) {
        int fd, r = 0;

        if ((fd = open_terminal("/dev/tty0", O_RDWR|O_NOCTTY|O_CLOEXEC)) < 0)
                return -errno;

        if (vt < 0) {
                int tiocl[2] = {
                        TIOCL_GETKMSGREDIRECT,
                        0
                };

                if (ioctl(fd, TIOCLINUX, tiocl) < 0) {
                        r = -errno;
                        goto fail;
                }

                vt = tiocl[0] <= 0 ? 1 : tiocl[0];
        }

        if (ioctl(fd, VT_ACTIVATE, vt) < 0)
                r = -errno;

fail:
        close_nointr_nofail(fd);
        return r;
}
