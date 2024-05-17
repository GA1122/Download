static int handle_rename(FsContext *ctx, const char *oldpath,
                         const char *newpath)
{
    errno = EOPNOTSUPP;
    return -1;
}
