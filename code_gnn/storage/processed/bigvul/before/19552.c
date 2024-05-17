SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events,
		int, maxevents, int, timeout)
{
	int error;
	struct file *file;
	struct eventpoll *ep;

	 
	if (maxevents <= 0 || maxevents > EP_MAX_EVENTS)
		return -EINVAL;

	 
	if (!access_ok(VERIFY_WRITE, events, maxevents * sizeof(struct epoll_event))) {
		error = -EFAULT;
		goto error_return;
	}

	 
	error = -EBADF;
	file = fget(epfd);
	if (!file)
		goto error_return;

	 
	error = -EINVAL;
	if (!is_file_epoll(file))
		goto error_fput;

	 
	ep = file->private_data;

	 
	error = ep_poll(ep, events, maxevents, timeout);

error_fput:
	fput(file);
error_return:

	return error;
}
