static int handle_truncate(FsContext *ctx, V9fsPath *fs_path, off_t size)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_NONBLOCK | O_WRONLY);
    if (fd < 0) {
        return fd;
    }
    ret = ftruncate(fd, size);
    close(fd);
    return ret;
}
