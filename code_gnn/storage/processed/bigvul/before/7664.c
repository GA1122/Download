static int handle_chmod(FsContext *fs_ctx, V9fsPath *fs_path, FsCred *credp)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)fs_ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_NONBLOCK);
    if (fd < 0) {
        return fd;
    }
    ret = fchmod(fd, credp->fc_mode);
    close(fd);
    return ret;
}
