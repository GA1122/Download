cifs_strict_writev(struct kiocb *iocb, const struct iovec *iov,
		   unsigned long nr_segs, loff_t pos)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	struct cifsInodeInfo *cinode = CIFS_I(inode);
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct cifsFileInfo *cfile = (struct cifsFileInfo *)
						iocb->ki_filp->private_data;
	struct cifs_tcon *tcon = tlink_tcon(cfile->tlink);
	ssize_t written;

	if (CIFS_CACHE_WRITE(cinode)) {
		if (cap_unix(tcon->ses) &&
		(CIFS_UNIX_FCNTL_CAP & le64_to_cpu(tcon->fsUnixInfo.Capability))
		    && ((cifs_sb->mnt_cifs_flags & CIFS_MOUNT_NOPOSIXBRL) == 0))
			return generic_file_aio_write(iocb, iov, nr_segs, pos);
		return cifs_writev(iocb, iov, nr_segs, pos);
	}
	 
	written = cifs_user_writev(iocb, iov, nr_segs, pos);
	if (written > 0 && CIFS_CACHE_READ(cinode)) {
		 
		cifs_invalidate_mapping(inode);
		cifs_dbg(FYI, "Set no oplock for inode=%p after a write operation\n",
			 inode);
		cinode->oplock = 0;
	}
	return written;
}
