static struct dirent *proxy_readdir(FsContext *ctx, V9fsFidOpenState *fs)
{
    return readdir(fs->dir.stream);
}
