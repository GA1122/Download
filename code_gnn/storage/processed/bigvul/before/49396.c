static int mem_open(struct inode *inode, struct file *file)
{
	int ret = __mem_open(inode, file, PTRACE_MODE_ATTACH);

	 
	file->f_mode |= FMODE_UNSIGNED_OFFSET;

	return ret;
}
