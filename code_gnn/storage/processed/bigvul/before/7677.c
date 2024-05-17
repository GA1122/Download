static int handle_lstat(FsContext *fs_ctx, V9fsPath *fs_path,
                        struct stat *stbuf)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)fs_ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_PATH);
    if (fd < 0) {
        return fd;
    }
    ret = fstatat(fd, "", stbuf, AT_EMPTY_PATH);
    close(fd);
    return ret;
}
