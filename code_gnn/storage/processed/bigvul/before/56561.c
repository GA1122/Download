static ssize_t ext4_direct_IO(struct kiocb *iocb, struct iov_iter *iter,
			      loff_t offset)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	size_t count = iov_iter_count(iter);
	ssize_t ret;

#ifdef CONFIG_EXT4_FS_ENCRYPTION
	if (ext4_encrypted_inode(inode) && S_ISREG(inode->i_mode))
		return 0;
#endif

	 
	if (ext4_should_journal_data(inode))
		return 0;

	 
	if (ext4_has_inline_data(inode))
		return 0;

	trace_ext4_direct_IO_enter(inode, offset, count, iov_iter_rw(iter));
	if (ext4_test_inode_flag(inode, EXT4_INODE_EXTENTS))
		ret = ext4_ext_direct_IO(iocb, iter, offset);
	else
		ret = ext4_ind_direct_IO(iocb, iter, offset);
	trace_ext4_direct_IO_exit(inode, offset, count, iov_iter_rw(iter), ret);
	return ret;
}