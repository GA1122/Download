int fd_cloexec(int fd, bool cloexec) {
        int flags;

        assert(fd >= 0);

        if ((flags = fcntl(fd, F_GETFD, 0)) < 0)
                return -errno;

        if (cloexec)
                flags |= FD_CLOEXEC;
        else
                flags &= ~FD_CLOEXEC;

        if (fcntl(fd, F_SETFD, flags) < 0)
                return -errno;

        return 0;
}
