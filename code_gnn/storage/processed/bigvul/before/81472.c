static inline int tracing_get_cpu(struct inode *inode)
{
	if (inode->i_cdev)  
		return (long)inode->i_cdev - 1;
	return RING_BUFFER_ALL_CPUS;
}
