static int handle_ioc_getversion(FsContext *ctx, V9fsPath *path,
                                 mode_t st_mode, uint64_t *st_gen)
{
#ifdef FS_IOC_GETVERSION
    int err;
    V9fsFidOpenState fid_open;

     
    if (!S_ISREG(st_mode) && !S_ISDIR(st_mode)) {
        errno = ENOTTY;
        return -1;
    }
    err = handle_open(ctx, path, O_RDONLY, &fid_open);
    if (err < 0) {
        return err;
    }
    err = ioctl(fid_open.fd, FS_IOC_GETVERSION, st_gen);
    handle_close(ctx, &fid_open);
    return err;
#else
    errno = ENOTTY;
    return -1;
#endif
}
