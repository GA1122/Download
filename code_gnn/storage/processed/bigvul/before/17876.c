static int local_set_cred_passthrough(FsContext *fs_ctx, int dirfd,
                                      const char *name, FsCred *credp)
{
    if (fchownat(dirfd, name, credp->fc_uid, credp->fc_gid,
                 AT_SYMLINK_NOFOLLOW) < 0) {
         
        if ((fs_ctx->export_flags & V9FS_SEC_MASK) != V9FS_SM_NONE) {
            return -1;
        }
    }

    return fchmodat_nofollow(dirfd, name, credp->fc_mode & 07777);
}
