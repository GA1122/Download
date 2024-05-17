int acquire_terminal(
                const char *name,
                AcquireTerminalFlags flags,
                usec_t timeout) {

        _cleanup_close_ int notify = -1, fd = -1;
        usec_t ts = USEC_INFINITY;
        int r, wd = -1;

        assert(name);
        assert(IN_SET(flags & ~ACQUIRE_TERMINAL_PERMISSIVE, ACQUIRE_TERMINAL_TRY, ACQUIRE_TERMINAL_FORCE, ACQUIRE_TERMINAL_WAIT));

         

        if ((flags & ~ACQUIRE_TERMINAL_PERMISSIVE) == ACQUIRE_TERMINAL_WAIT) {
                notify = inotify_init1(IN_CLOEXEC | (timeout != USEC_INFINITY ? IN_NONBLOCK : 0));
                if (notify < 0)
                        return -errno;

                wd = inotify_add_watch(notify, name, IN_CLOSE);
                if (wd < 0)
                        return -errno;

                if (timeout != USEC_INFINITY)
                        ts = now(CLOCK_MONOTONIC);
        }

        for (;;) {
                struct sigaction sa_old, sa_new = {
                        .sa_handler = SIG_IGN,
                        .sa_flags = SA_RESTART,
                };

                if (notify >= 0) {
                        r = flush_fd(notify);
                        if (r < 0)
                                return r;
                }

                 
                fd = open_terminal(name, O_RDWR|O_NOCTTY|O_CLOEXEC);
                if (fd < 0)
                        return fd;

                 
                assert_se(sigaction(SIGHUP, &sa_new, &sa_old) == 0);

                 
                r = ioctl(fd, TIOCSCTTY,
                          (flags & ~ACQUIRE_TERMINAL_PERMISSIVE) == ACQUIRE_TERMINAL_FORCE) < 0 ? -errno : 0;

                 
                assert_se(sigaction(SIGHUP, &sa_old, NULL) == 0);

                 
                if (r >= 0)
                        break;

                 
                if (r != -EPERM)
                        return r;

                if (flags & ACQUIRE_TERMINAL_PERMISSIVE)  
                        break;

                if (flags != ACQUIRE_TERMINAL_WAIT)  
                        return r;

                assert(notify >= 0);
                assert(wd >= 0);

                for (;;) {
                        union inotify_event_buffer buffer;
                        struct inotify_event *e;
                        ssize_t l;

                        if (timeout != USEC_INFINITY) {
                                usec_t n;

                                assert(ts != USEC_INFINITY);

                                n = now(CLOCK_MONOTONIC);
                                if (ts + timeout < n)
                                        return -ETIMEDOUT;

                                r = fd_wait_for_event(notify, POLLIN, ts + timeout - n);
                                if (r < 0)
                                        return r;
                                if (r == 0)
                                        return -ETIMEDOUT;
                        }

                        l = read(notify, &buffer, sizeof(buffer));
                        if (l < 0) {
                                if (IN_SET(errno, EINTR, EAGAIN))
                                        continue;

                                return -errno;
                        }

                        FOREACH_INOTIFY_EVENT(e, buffer, l) {
                                if (e->mask & IN_Q_OVERFLOW)  
                                        break;

                                if (e->wd != wd || !(e->mask & IN_CLOSE))  
                                        return -EIO;
                        }

                        break;
                }

                 
                fd = safe_close(fd);
        }

        return TAKE_FD(fd);
}
