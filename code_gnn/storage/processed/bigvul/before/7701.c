static inline int name_to_handle(int dirfd, const char *name,
                                 struct file_handle *fh, int *mnt_id, int flags)
{
    return name_to_handle_at(dirfd, name, fh, mnt_id, flags);
}
