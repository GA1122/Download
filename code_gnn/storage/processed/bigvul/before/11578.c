int release_terminal(void) {
        int r = 0, fd;
        struct sigaction sa_old, sa_new;

        if ((fd = open("/dev/tty", O_RDWR|O_NOCTTY|O_NDELAY|O_CLOEXEC)) < 0)
                return -errno;

         

        zero(sa_new);
        sa_new.sa_handler = SIG_IGN;
        sa_new.sa_flags = SA_RESTART;
        assert_se(sigaction(SIGHUP, &sa_new, &sa_old) == 0);

        if (ioctl(fd, TIOCNOTTY) < 0)
                r = -errno;

        assert_se(sigaction(SIGHUP, &sa_old, NULL) == 0);

        close_nointr_nofail(fd);
        return r;
}
