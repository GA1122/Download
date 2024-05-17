static ssize_t ocfs2_file_read_iter(struct kiocb *iocb,
				   struct iov_iter *to)
{
	int ret = 0, rw_level = -1, lock_level = 0;
	struct file *filp = iocb->ki_filp;
	struct inode *inode = file_inode(filp);

	trace_ocfs2_file_aio_read(inode, filp, filp->f_path.dentry,
			(unsigned long long)OCFS2_I(inode)->ip_blkno,
			filp->f_path.dentry->d_name.len,
			filp->f_path.dentry->d_name.name,
			to->nr_segs);	 


	if (!inode) {
		ret = -EINVAL;
		mlog_errno(ret);
		goto bail;
	}

	 
	if (iocb->ki_flags & IOCB_DIRECT) {
		ret = ocfs2_rw_lock(inode, 0);
		if (ret < 0) {
			mlog_errno(ret);
			goto bail;
		}
		rw_level = 0;
		 
		ocfs2_iocb_set_rw_locked(iocb, rw_level);
	}

	 
	ret = ocfs2_inode_lock_atime(inode, filp->f_path.mnt, &lock_level);
	if (ret < 0) {
		mlog_errno(ret);
		goto bail;
	}
	ocfs2_inode_unlock(inode, lock_level);

	ret = generic_file_read_iter(iocb, to);
	trace_generic_file_aio_read_ret(ret);

	 
	BUG_ON(ret == -EIOCBQUEUED && !(iocb->ki_flags & IOCB_DIRECT));

	 
	if (ret == -EIOCBQUEUED || !ocfs2_iocb_is_rw_locked(iocb)) {
		rw_level = -1;
	}

bail:
	if (rw_level != -1)
		ocfs2_rw_unlock(inode, rw_level);

	return ret;
}
