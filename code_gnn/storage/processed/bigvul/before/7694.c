static int handle_statfs(FsContext *ctx, V9fsPath *fs_path,
                         struct statfs *stbuf)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_NONBLOCK);
    if (fd < 0) {
        return fd;
    }
    ret = fstatfs(fd, stbuf);
    close(fd);
    return ret;
}
