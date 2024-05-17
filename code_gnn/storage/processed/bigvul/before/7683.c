static int handle_opendir(FsContext *ctx,
                          V9fsPath *fs_path, V9fsFidOpenState *fs)
{
    int ret;
    ret = handle_open(ctx, fs_path, O_DIRECTORY, fs);
    if (ret < 0) {
        return -1;
    }
    fs->dir.stream = fdopendir(ret);
    if (!fs->dir.stream) {
        return -1;
    }
    return 0;
}
