int make_console_stdio(void) {
        int fd, r;

         

        fd = acquire_terminal("/dev/console", ACQUIRE_TERMINAL_FORCE|ACQUIRE_TERMINAL_PERMISSIVE, USEC_INFINITY);
        if (fd < 0)
                return log_error_errno(fd, "Failed to acquire terminal: %m");

        r = reset_terminal_fd(fd, true);
        if (r < 0)
                log_warning_errno(r, "Failed to reset terminal, ignoring: %m");

        r = rearrange_stdio(fd, fd, fd);  
        if (r < 0)
                return log_error_errno(r, "Failed to make terminal stdin/stdout/stderr: %m");

        reset_terminal_feature_caches();

        return 0;
}