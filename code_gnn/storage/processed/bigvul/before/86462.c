static unsigned int userfaultfd_poll(struct file *file, poll_table *wait)
{
	struct userfaultfd_ctx *ctx = file->private_data;
	unsigned int ret;

	poll_wait(file, &ctx->fd_wqh, wait);

	switch (ctx->state) {
	case UFFD_STATE_WAIT_API:
		return POLLERR;
	case UFFD_STATE_RUNNING:
		 
		if (unlikely(!(file->f_flags & O_NONBLOCK)))
			return POLLERR;
		 
		ret = 0;
		smp_mb();
		if (waitqueue_active(&ctx->fault_pending_wqh))
			ret = POLLIN;
		else if (waitqueue_active(&ctx->event_wqh))
			ret = POLLIN;

		return ret;
	default:
		WARN_ON_ONCE(1);
		return POLLERR;
	}
}
