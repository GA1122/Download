static int ocfs2_dio_end_io(struct kiocb *iocb,
			    loff_t offset,
			    ssize_t bytes,
			    void *private)
{
	struct inode *inode = file_inode(iocb->ki_filp);
	int level;
	int ret = 0;

	 
	BUG_ON(!ocfs2_iocb_is_rw_locked(iocb));

	if (bytes > 0 && private)
		ret = ocfs2_dio_end_io_write(inode, private, offset, bytes);

	ocfs2_iocb_clear_rw_locked(iocb);

	level = ocfs2_iocb_rw_locked_level(iocb);
	ocfs2_rw_unlock(inode, level);
	return ret;
}