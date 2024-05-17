static int fifo_open(struct inode *inode, struct file *filp)
{
	struct pipe_inode_info *pipe;
	bool is_pipe = inode->i_sb->s_magic == PIPEFS_MAGIC;
	int ret;

	filp->f_version = 0;

	spin_lock(&inode->i_lock);
	if (inode->i_pipe) {
		pipe = inode->i_pipe;
		pipe->files++;
		spin_unlock(&inode->i_lock);
	} else {
		spin_unlock(&inode->i_lock);
		pipe = alloc_pipe_info();
		if (!pipe)
			return -ENOMEM;
		pipe->files = 1;
		spin_lock(&inode->i_lock);
		if (unlikely(inode->i_pipe)) {
			inode->i_pipe->files++;
			spin_unlock(&inode->i_lock);
			free_pipe_info(pipe);
			pipe = inode->i_pipe;
		} else {
			inode->i_pipe = pipe;
			spin_unlock(&inode->i_lock);
		}
	}
	filp->private_data = pipe;
	 

	__pipe_lock(pipe);

	 
	filp->f_mode &= (FMODE_READ | FMODE_WRITE);

	switch (filp->f_mode) {
	case FMODE_READ:
	 
		pipe->r_counter++;
		if (pipe->readers++ == 0)
			wake_up_partner(pipe);

		if (!is_pipe && !pipe->writers) {
			if ((filp->f_flags & O_NONBLOCK)) {
				 
				filp->f_version = pipe->w_counter;
			} else {
				if (wait_for_partner(pipe, &pipe->w_counter))
					goto err_rd;
			}
		}
		break;
	
	case FMODE_WRITE:
	 
		ret = -ENXIO;
		if (!is_pipe && (filp->f_flags & O_NONBLOCK) && !pipe->readers)
			goto err;

		pipe->w_counter++;
		if (!pipe->writers++)
			wake_up_partner(pipe);

		if (!is_pipe && !pipe->readers) {
			if (wait_for_partner(pipe, &pipe->r_counter))
				goto err_wr;
		}
		break;
	
	case FMODE_READ | FMODE_WRITE:
	 

		pipe->readers++;
		pipe->writers++;
		pipe->r_counter++;
		pipe->w_counter++;
		if (pipe->readers == 1 || pipe->writers == 1)
			wake_up_partner(pipe);
		break;

	default:
		ret = -EINVAL;
		goto err;
	}

	 
	__pipe_unlock(pipe);
	return 0;

err_rd:
	if (!--pipe->readers)
		wake_up_interruptible(&pipe->wait);
	ret = -ERESTARTSYS;
	goto err;

err_wr:
	if (!--pipe->writers)
		wake_up_interruptible(&pipe->wait);
	ret = -ERESTARTSYS;
	goto err;

err:
	__pipe_unlock(pipe);

	put_pipe_info(inode, pipe);
	return ret;
}
