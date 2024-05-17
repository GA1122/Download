int default_signals(int sig, ...) {
        struct sigaction sa;
        va_list ap;
        int r = 0;

        zero(sa);
        sa.sa_handler = SIG_DFL;
        sa.sa_flags = SA_RESTART;

        if (sigaction(sig, &sa, NULL) < 0)
                r = -errno;

        va_start(ap, sig);
        while ((sig = va_arg(ap, int)) > 0)
                if (sigaction(sig, &sa, NULL) < 0)
                        r = -errno;
        va_end(ap);

        return r;
}
