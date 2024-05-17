static int handle_lremovexattr(FsContext *ctx, V9fsPath *fs_path,
                               const char *name)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_NONBLOCK);
    if (fd < 0) {
        return fd;
    }
    ret = fremovexattr(fd, name);
    close(fd);
    return ret;
}
