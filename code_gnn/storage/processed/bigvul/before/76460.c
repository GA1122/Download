static __poll_t userfaultfd_poll(struct file *file, poll_table *wait)
{
	struct userfaultfd_ctx *ctx = file->private_data;
	__poll_t ret;

	poll_wait(file, &ctx->fd_wqh, wait);

	switch (ctx->state) {
	case UFFD_STATE_WAIT_API:
		return EPOLLERR;
	case UFFD_STATE_RUNNING:
		 
		if (unlikely(!(file->f_flags & O_NONBLOCK)))
			return EPOLLERR;
		 
		ret = 0;
		smp_mb();
		if (waitqueue_active(&ctx->fault_pending_wqh))
			ret = EPOLLIN;
		else if (waitqueue_active(&ctx->event_wqh))
			ret = EPOLLIN;

		return ret;
	default:
		WARN_ON_ONCE(1);
		return EPOLLERR;
	}
}
