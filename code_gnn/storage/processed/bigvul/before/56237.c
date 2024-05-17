static char *ovl_read_symlink(struct dentry *realdentry)
{
	int res;
	char *buf;
	struct inode *inode = realdentry->d_inode;
	mm_segment_t old_fs;

	res = -EINVAL;
	if (!inode->i_op->readlink)
		goto err;

	res = -ENOMEM;
	buf = (char *) __get_free_page(GFP_KERNEL);
	if (!buf)
		goto err;

	old_fs = get_fs();
	set_fs(get_ds());
	 
	res = inode->i_op->readlink(realdentry,
				    (char __user *)buf, PAGE_SIZE - 1);
	set_fs(old_fs);
	if (res < 0) {
		free_page((unsigned long) buf);
		goto err;
	}
	buf[res] = '\0';

	return buf;

err:
	return ERR_PTR(res);
}
