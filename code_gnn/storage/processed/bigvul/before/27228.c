SYSCALL_DEFINE3(osf_readv, unsigned long, fd,
		const struct iovec __user *, vector, unsigned long, count)
{
	if (unlikely(personality(current->personality) == PER_OSF4))
		if (osf_fix_iov_len(vector, count))
			return -EFAULT;
	return sys_readv(fd, vector, count);
}
