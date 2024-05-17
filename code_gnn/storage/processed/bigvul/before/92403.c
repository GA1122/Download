int ptsname_namespace(int pty, char **ret) {
        int no = -1, r;

         

        r = ioctl(pty, TIOCGPTN, &no);
        if (r < 0)
                return -errno;

        if (no < 0)
                return -EIO;

        if (asprintf(ret, "/dev/pts/%i", no) < 0)
                return -ENOMEM;

        return 0;
}
