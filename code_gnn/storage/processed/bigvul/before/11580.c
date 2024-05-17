int reset_all_signal_handlers(void) {
        int sig;

        for (sig = 1; sig < _NSIG; sig++) {
                struct sigaction sa;

                if (sig == SIGKILL || sig == SIGSTOP)
                        continue;

                zero(sa);
                sa.sa_handler = SIG_DFL;
                sa.sa_flags = SA_RESTART;

                 
                if ((sigaction(sig, &sa, NULL) < 0))
                        if (errno != EINVAL)
                                return -errno;
        }

        return 0;
}
