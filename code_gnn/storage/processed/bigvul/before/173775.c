static void watch_package_list(struct fuse_global* global) {
 struct inotify_event *event;
 char event_buf[512];

 int nfd = inotify_init();
 if (nfd < 0) {
        ERROR("inotify_init failed: %s\n", strerror(errno));
 return;
 }

 bool active = false;
 while (1) {
 if (!active) {
 int res = inotify_add_watch(nfd, kPackagesListFile, IN_DELETE_SELF);
 if (res == -1) {
 if (errno == ENOENT || errno == EACCES) {
  
                    ERROR("missing packages.list; retrying\n");
                    sleep(3);
 continue;
 } else {
                    ERROR("inotify_add_watch failed: %s\n", strerror(errno));
 return;
 }
 }

  
 if (read_package_list(global) == -1) {
                ERROR("read_package_list failed: %s\n", strerror(errno));
 return;
 }
            active = true;
 }

 int event_pos = 0;
 int res = read(nfd, event_buf, sizeof(event_buf));
 if (res < (int) sizeof(*event)) {
 if (errno == EINTR)
 continue;
            ERROR("failed to read inotify event: %s\n", strerror(errno));
 return;
 }

 while (res >= (int) sizeof(*event)) {
 int event_size;
            event = (struct inotify_event *) (event_buf + event_pos);

            TRACE("inotify event: %08x\n", event->mask);
 if ((event->mask & IN_IGNORED) == IN_IGNORED) {
  
                active = false;
 }

            event_size = sizeof(*event) + event->len;
            res -= event_size;
            event_pos += event_size;
 }
 }
}
