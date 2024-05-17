int open_check_o_direct(struct file *f)
{
	 
	if (f->f_flags & O_DIRECT) {
		if (!f->f_mapping->a_ops || !f->f_mapping->a_ops->direct_IO)
			return -EINVAL;
	}
	return 0;
}
