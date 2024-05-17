static ssize_t aio_run_iocb(struct kiocb *iocb)
{
	struct kioctx	*ctx = iocb->ki_ctx;
	ssize_t (*retry)(struct kiocb *);
	ssize_t ret;

	if (!(retry = iocb->ki_retry)) {
		printk("aio_run_iocb: iocb->ki_retry = NULL\n");
		return 0;
	}

	 

	kiocbClearKicked(iocb);

	 
	iocb->ki_run_list.next = iocb->ki_run_list.prev = NULL;
	spin_unlock_irq(&ctx->ctx_lock);

	 
	if (kiocbIsCancelled(iocb)) {
		ret = -EINTR;
		aio_complete(iocb, ret, 0);
		 
		goto out;
	}

	 
	ret = retry(iocb);

	if (ret != -EIOCBRETRY && ret != -EIOCBQUEUED) {
		 
		if (unlikely(ret == -ERESTARTSYS || ret == -ERESTARTNOINTR ||
			     ret == -ERESTARTNOHAND || ret == -ERESTART_RESTARTBLOCK))
			ret = -EINTR;
		aio_complete(iocb, ret, 0);
	}
out:
	spin_lock_irq(&ctx->ctx_lock);

	if (-EIOCBRETRY == ret) {
		 

		 
		INIT_LIST_HEAD(&iocb->ki_run_list);
		 
		if (kiocbIsKicked(iocb)) {
			__queue_kicked_iocb(iocb);

			 
			aio_queue_work(ctx);
		}
	}
	return ret;
}
