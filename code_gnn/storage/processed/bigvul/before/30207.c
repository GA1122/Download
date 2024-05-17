ftrace_profile_read(struct file *filp, char __user *ubuf,
		     size_t cnt, loff_t *ppos)
{
	char buf[64];		 
	int r;

	r = sprintf(buf, "%u\n", ftrace_profile_enabled);
	return simple_read_from_buffer(ubuf, cnt, ppos, buf, r);
}
