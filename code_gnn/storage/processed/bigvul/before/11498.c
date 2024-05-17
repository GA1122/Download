int acquire_terminal(const char *name, bool fail, bool force, bool ignore_tiocstty_eperm) {
        int fd = -1, notify = -1, r, wd = -1;

        assert(name);

         

        if (!fail && !force) {
                if ((notify = inotify_init1(IN_CLOEXEC)) < 0) {
                        r = -errno;
                        goto fail;
                }

                if ((wd = inotify_add_watch(notify, name, IN_CLOSE)) < 0) {
                        r = -errno;
                        goto fail;
                }
        }

        for (;;) {
                if (notify >= 0)
                        if ((r = flush_fd(notify)) < 0)
                                goto fail;

                 
                if ((fd = open_terminal(name, O_RDWR|O_NOCTTY|O_CLOEXEC)) < 0)
                        return fd;

                 
                r = ioctl(fd, TIOCSCTTY, force);

                 
                if (r < 0 && errno == EPERM && ignore_tiocstty_eperm)
                        r = 0;

                if (r < 0 && (force || fail || errno != EPERM)) {
                        r = -errno;
                        goto fail;
                }

                if (r >= 0)
                        break;

                assert(!fail);
                assert(!force);
                assert(notify >= 0);

                for (;;) {
                        uint8_t inotify_buffer[sizeof(struct inotify_event) + FILENAME_MAX];
                        ssize_t l;
                        struct inotify_event *e;

                        if ((l = read(notify, inotify_buffer, sizeof(inotify_buffer))) < 0) {

                                if (errno == EINTR)
                                        continue;

                                r = -errno;
                                goto fail;
                        }

                        e = (struct inotify_event*) inotify_buffer;

                        while (l > 0) {
                                size_t step;

                                if (e->wd != wd || !(e->mask & IN_CLOSE)) {
                                        r = -EIO;
                                        goto fail;
                                }

                                step = sizeof(struct inotify_event) + e->len;
                                assert(step <= (size_t) l);

                                e = (struct inotify_event*) ((uint8_t*) e + step);
                                l -= step;
                        }

                        break;
                }

                 
                close_nointr_nofail(fd);
        }

        if (notify >= 0)
                close_nointr_nofail(notify);

        r = reset_terminal_fd(fd, true);
        if (r < 0)
                log_warning("Failed to reset terminal: %s", strerror(-r));

        return fd;

fail:
        if (fd >= 0)
                close_nointr_nofail(fd);

        if (notify >= 0)
                close_nointr_nofail(notify);

        return r;
}
