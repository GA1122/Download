ftrace_notrace_open(struct inode *inode, struct file *file)
{
	return ftrace_regex_open(&global_ops, FTRACE_ITER_NOTRACE,
				 inode, file);
 }