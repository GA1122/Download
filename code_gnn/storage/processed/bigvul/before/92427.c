static int verify_vc_allocation_byfd(int fd) {
        struct vt_stat vcs = {};

        if (ioctl(fd, VT_GETSTATE, &vcs) < 0)
                return -errno;

         return verify_vc_allocation(vcs.v_active);
 }