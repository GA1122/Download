int vt_disallocate(const char *name) {
        _cleanup_close_ int fd = -1;
        const char *e, *n;
        unsigned u;
        int r;

         

        e = path_startswith(name, "/dev/");
        if (!e)
                return -EINVAL;

        if (!tty_is_vc(name)) {
                 

                fd = open_terminal(name, O_RDWR|O_NOCTTY|O_CLOEXEC);
                if (fd < 0)
                        return fd;

                loop_write(fd,
                           "\033[r"     
                           "\033[H"     
                           "\033[2J",   
                           10, false);
                return 0;
        }

        n = startswith(e, "tty");
        if (!n)
                return -EINVAL;

        r = safe_atou(n, &u);
        if (r < 0)
                return r;

        if (u <= 0)
                return -EINVAL;

         
        fd = open_terminal("/dev/tty0", O_RDWR|O_NOCTTY|O_CLOEXEC|O_NONBLOCK);
        if (fd < 0)
                return fd;

        r = ioctl(fd, VT_DISALLOCATE, u);
        fd = safe_close(fd);

        if (r >= 0)
                return 0;

        if (errno != EBUSY)
                return -errno;

         
        fd = open_terminal(name, O_RDWR|O_NOCTTY|O_CLOEXEC);
        if (fd < 0)
                return fd;

        loop_write(fd,
                   "\033[r"    
                   "\033[H"    
                   "\033[3J",  
                   10, false);
        return 0;
}
