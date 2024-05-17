static inline int open_by_handle(int mountfd, const char *fh, int flags)
{
    return open_by_handle_at(mountfd, (struct file_handle *)fh, flags);
}
