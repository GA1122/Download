int open_terminal(const char *name, int mode) {
        unsigned c = 0;
        int fd;

         

        if (mode & O_CREAT)
                return -EINVAL;

        for (;;) {
                fd = open(name, mode, 0);
                if (fd >= 0)
                        break;

                if (errno != EIO)
                        return -errno;

                 
                if (c >= 20)
                        return -errno;

                usleep(50 * USEC_PER_MSEC);
                c++;
        }

        if (isatty(fd) <= 0) {
                safe_close(fd);
                return -ENOTTY;
        }

        return fd;
}
