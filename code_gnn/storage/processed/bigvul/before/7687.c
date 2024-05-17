static struct dirent *handle_readdir(FsContext *ctx, V9fsFidOpenState *fs)
{
    return readdir(fs->dir.stream);
}
