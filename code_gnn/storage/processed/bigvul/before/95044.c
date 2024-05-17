SYSCALL_DEFINE1(io_destroy, aio_context_t, ctx)
{
	struct kioctx *ioctx = lookup_ioctx(ctx);
	if (likely(NULL != ioctx)) {
		struct completion requests_done =
			COMPLETION_INITIALIZER_ONSTACK(requests_done);
		int ret;

		 
		ret = kill_ioctx(current->mm, ioctx, &requests_done);
		percpu_ref_put(&ioctx->users);

		 
		if (!ret)
			wait_for_completion(&requests_done);

		return ret;
	}
	pr_debug("EINVAL: invalid context id\n");
	return -EINVAL;
}
