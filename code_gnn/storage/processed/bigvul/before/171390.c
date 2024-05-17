asocket* create_local_service_socket(const char* name, const atransport* transport) {
#if !ADB_HOST
 if (!strcmp(name, "jdwp")) {
 return create_jdwp_service_socket();
 }
 if (!strcmp(name, "track-jdwp")) {
 return create_jdwp_tracker_service_socket();
 }
#endif
 int fd = service_to_fd(name, transport);
 if (fd < 0) {
 return 0;
 }

    asocket* s = create_local_socket(fd);
    D("LS(%d): bound to '%s' via %d", s->id, name, fd);

#if !ADB_HOST
 char debug[PROPERTY_VALUE_MAX];
 if (!strncmp(name, "root:", 5)) {
        property_get("ro.debuggable", debug, "");
 }

 if ((!strncmp(name, "root:", 5) && getuid() != 0 && strcmp(debug, "1") == 0) ||
 (!strncmp(name, "unroot:", 7) && getuid() == 0) ||
 !strncmp(name, "usb:", 4) ||
 !strncmp(name, "tcpip:", 6)) {
        D("LS(%d): enabling exit_on_close", s->id);
        s->exit_on_close = 1;
 }
#endif

 return s;
}
