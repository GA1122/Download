static ssize_t ext4_ext_direct_IO(struct kiocb *iocb, struct iov_iter *iter,
				  loff_t offset)
{
	struct file *file = iocb->ki_filp;
	struct inode *inode = file->f_mapping->host;
	ssize_t ret;
	size_t count = iov_iter_count(iter);
	int overwrite = 0;
	get_block_t *get_block_func = NULL;
	int dio_flags = 0;
	loff_t final_size = offset + count;
	ext4_io_end_t *io_end = NULL;

	 
	if (iov_iter_rw(iter) != WRITE || final_size > inode->i_size)
		return ext4_ind_direct_IO(iocb, iter, offset);

	BUG_ON(iocb->private == NULL);

	 
	if (iov_iter_rw(iter) == WRITE)
		inode_dio_begin(inode);

	 
	overwrite = *((int *)iocb->private);

	if (overwrite) {
		down_read(&EXT4_I(inode)->i_data_sem);
		mutex_unlock(&inode->i_mutex);
	}

	 
	iocb->private = NULL;
	ext4_inode_aio_set(inode, NULL);
	if (!is_sync_kiocb(iocb)) {
		io_end = ext4_init_io_end(inode, GFP_NOFS);
		if (!io_end) {
			ret = -ENOMEM;
			goto retake_lock;
		}
		 
		iocb->private = ext4_get_io_end(io_end);
		 
		ext4_inode_aio_set(inode, io_end);
	}

	if (overwrite) {
		get_block_func = ext4_get_block_write_nolock;
	} else {
		get_block_func = ext4_get_block_write;
		dio_flags = DIO_LOCKING;
	}
#ifdef CONFIG_EXT4_FS_ENCRYPTION
	BUG_ON(ext4_encrypted_inode(inode) && S_ISREG(inode->i_mode));
#endif
	if (IS_DAX(inode))
		ret = dax_do_io(iocb, inode, iter, offset, get_block_func,
				ext4_end_io_dio, dio_flags);
	else
		ret = __blockdev_direct_IO(iocb, inode,
					   inode->i_sb->s_bdev, iter, offset,
					   get_block_func,
					   ext4_end_io_dio, NULL, dio_flags);

	 
	if (io_end) {
		ext4_inode_aio_set(inode, NULL);
		ext4_put_io_end(io_end);
		 
		if (ret <= 0 && ret != -EIOCBQUEUED && iocb->private) {
			WARN_ON(iocb->private != io_end);
			WARN_ON(io_end->flag & EXT4_IO_END_UNWRITTEN);
			ext4_put_io_end(io_end);
			iocb->private = NULL;
		}
	}
	if (ret > 0 && !overwrite && ext4_test_inode_state(inode,
						EXT4_STATE_DIO_UNWRITTEN)) {
		int err;
		 
		err = ext4_convert_unwritten_extents(NULL, inode,
						     offset, ret);
		if (err < 0)
			ret = err;
		ext4_clear_inode_state(inode, EXT4_STATE_DIO_UNWRITTEN);
	}

retake_lock:
	if (iov_iter_rw(iter) == WRITE)
		inode_dio_end(inode);
	 
	if (overwrite) {
		up_read(&EXT4_I(inode)->i_data_sem);
		mutex_lock(&inode->i_mutex);
	}

	return ret;
}
