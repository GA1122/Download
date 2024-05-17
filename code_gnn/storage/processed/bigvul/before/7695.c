static int handle_symlink(FsContext *fs_ctx, const char *oldpath,
                          V9fsPath *dir_path, const char *name, FsCred *credp)
{
    int fd, dirfd, ret;
    struct handle_data *data = (struct handle_data *)fs_ctx->private;

    dirfd = open_by_handle(data->mountfd, dir_path->data, O_PATH);
    if (dirfd < 0) {
        return dirfd;
    }
    ret = symlinkat(oldpath, dirfd, name);
    if (!ret) {
        fd = openat(dirfd, name, O_PATH | O_NOFOLLOW);
        if (fd < 0) {
            ret = fd;
            goto err_out;
        }
        ret = fchownat(fd, "", credp->fc_uid, credp->fc_gid, AT_EMPTY_PATH);
        close(fd);
    }
err_out:
    close(dirfd);
    return ret;
}
