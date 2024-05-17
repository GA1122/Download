static int handle_unlinkat(FsContext *ctx, V9fsPath *dir,
                           const char *name, int flags)
{
    int dirfd, ret;
    struct handle_data *data = (struct handle_data *)ctx->private;
    int rflags;

    dirfd = open_by_handle(data->mountfd, dir->data, O_PATH);
    if (dirfd < 0) {
        return dirfd;
    }

    rflags = 0;
    if (flags & P9_DOTL_AT_REMOVEDIR) {
        rflags |= AT_REMOVEDIR;
    }

    ret = unlinkat(dirfd, name, rflags);

    close(dirfd);
    return ret;
}
