static int handle_chown(FsContext *fs_ctx, V9fsPath *fs_path, FsCred *credp)
{
    int fd, ret;
    struct handle_data *data = (struct handle_data *)fs_ctx->private;

    fd = open_by_handle(data->mountfd, fs_path->data, O_PATH);
    if (fd < 0) {
        return fd;
    }
    ret = fchownat(fd, "", credp->fc_uid, credp->fc_gid, AT_EMPTY_PATH);
    close(fd);
    return ret;
}
