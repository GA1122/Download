static int handle_open2(FsContext *fs_ctx, V9fsPath *dir_path, const char *name,
                        int flags, FsCred *credp, V9fsFidOpenState *fs)
{
    int ret;
    int dirfd, fd;
    struct handle_data *data = (struct handle_data *)fs_ctx->private;

    dirfd = open_by_handle(data->mountfd, dir_path->data, O_PATH);
    if (dirfd < 0) {
        return dirfd;
    }
    fd = openat(dirfd, name, flags | O_NOFOLLOW, credp->fc_mode);
    if (fd >= 0) {
        ret = handle_update_file_cred(dirfd, name, credp);
        if (ret < 0) {
            close(fd);
            fd = ret;
        } else {
            fs->fd = fd;
        }
    }
    close(dirfd);
    return fd;
}
