cifs_strict_readv(struct kiocb *iocb, const struct iovec *iov,
		  unsigned long nr_segs, loff_t pos)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	struct cifsInodeInfo *cinode = CIFS_I(inode);
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct cifsFileInfo *cfile = (struct cifsFileInfo *)
						iocb->ki_filp->private_data;
	struct cifs_tcon *tcon = tlink_tcon(cfile->tlink);
	int rc = -EACCES;

	 
	if (!CIFS_CACHE_READ(cinode))
		return cifs_user_readv(iocb, iov, nr_segs, pos);

	if (cap_unix(tcon->ses) &&
	    (CIFS_UNIX_FCNTL_CAP & le64_to_cpu(tcon->fsUnixInfo.Capability)) &&
	    ((cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NOPOSIXBRL) == 0))
		return generic_file_aio_read(iocb, iov, nr_segs, pos);

	 
	down_read(&cinode->lock_sem);
	if (!cifs_find_lock_conflict(cfile, pos, iov_length(iov, nr_segs),
				     tcon->ses->server->vals->shared_lock_type,
				     NULL, CIFS_READ_OP))
		rc = generic_file_aio_read(iocb, iov, nr_segs, pos);
	up_read(&cinode->lock_sem);
	return rc;
}
