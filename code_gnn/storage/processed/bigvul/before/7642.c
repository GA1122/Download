static int proxy_parse_opts(QemuOpts *opts, struct FsDriverEntry *fs)
{
    const char *socket = qemu_opt_get(opts, "socket");
    const char *sock_fd = qemu_opt_get(opts, "sock_fd");

    if (!socket && !sock_fd) {
        error_report("Must specify either socket or sock_fd");
        return -1;
    }
    if (socket && sock_fd) {
        error_report("Both socket and sock_fd options specified");
        return -1;
    }
    if (socket) {
        fs->path = g_strdup(socket);
        fs->export_flags = V9FS_PROXY_SOCK_NAME;
    } else {
        fs->path = g_strdup(sock_fd);
        fs->export_flags = V9FS_PROXY_SOCK_FD;
    }
    return 0;
}
