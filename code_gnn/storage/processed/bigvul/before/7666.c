static int handle_close(FsContext *ctx, V9fsFidOpenState *fs)
{
    return close(fs->fd);
}
