static int handle_lsetxattr(FsContext *ctx, V9fsPath *fs_path, const char *name,
                            void *value, size_t size, int flags)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_NONBLOCK);
    if (fd < 0) {
        return fd;
    }
    ret = fsetxattr(fd, name, value, size, flags);
    close(fd);
    return ret;
}
