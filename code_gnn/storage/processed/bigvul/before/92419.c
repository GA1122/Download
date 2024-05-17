int vt_release(int fd, bool restore) {
        assert(fd >= 0);

         

        if (ioctl(fd, VT_RELDISP, 1) < 0)
                return -errno;

        if (restore)
                return vt_restore(fd);

        return 0;
}
