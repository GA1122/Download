int local_open_nofollow(FsContext *fs_ctx, const char *path, int flags,
                        mode_t mode)
{
    LocalData *data = fs_ctx->private;

     
    while (*path == '/') {
        path++;
    }

    return relative_openat_nofollow(data->mountfd, path, flags, mode);
}
