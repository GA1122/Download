tracing_free_buffer_write(struct file *filp, const char __user *ubuf,
			  size_t cnt, loff_t *ppos)
{
	 

	*ppos += cnt;

	return cnt;
}
