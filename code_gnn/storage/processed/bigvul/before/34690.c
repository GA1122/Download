static ssize_t gfs2_file_aio_write(struct kiocb *iocb, const struct iovec *iov,
				   unsigned long nr_segs, loff_t pos)
{
	struct file *file = iocb->ki_filp;

	if (file->f_flags & O_APPEND) {
		struct dentry *dentry = file->f_dentry;
		struct gfs2_inode *ip = GFS2_I(dentry->d_inode);
		struct gfs2_holder gh;
		int ret;

		ret = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, 0, &gh);
		if (ret)
			return ret;
		gfs2_glock_dq_uninit(&gh);
	}

 	return generic_file_aio_write(iocb, iov, nr_segs, pos);
 }