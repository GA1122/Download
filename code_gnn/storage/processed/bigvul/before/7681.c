static int handle_open(FsContext *ctx, V9fsPath *fs_path,
                       int flags, V9fsFidOpenState *fs)
{
    struct handle_data *data = (struct handle_data *)ctx->private;

    fs->fd = open_by_handle(data->mountfd, fs_path->data, flags);
    return fs->fd;
}
