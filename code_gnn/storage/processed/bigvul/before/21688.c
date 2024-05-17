static int io_submit_one(struct kioctx *ctx, struct iocb __user *user_iocb,
			 struct iocb *iocb, struct kiocb_batch *batch,
			 bool compat)
{
	struct kiocb *req;
	struct file *file;
	ssize_t ret;

	 
	if (unlikely(iocb->aio_reserved1 || iocb->aio_reserved2)) {
		pr_debug("EINVAL: io_submit: reserve field set\n");
		return -EINVAL;
	}

	 
	if (unlikely(
	    (iocb->aio_buf != (unsigned long)iocb->aio_buf) ||
	    (iocb->aio_nbytes != (size_t)iocb->aio_nbytes) ||
	    ((ssize_t)iocb->aio_nbytes < 0)
	   )) {
		pr_debug("EINVAL: io_submit: overflow check\n");
		return -EINVAL;
	}

	file = fget(iocb->aio_fildes);
	if (unlikely(!file))
		return -EBADF;

	req = aio_get_req(ctx, batch);   
	if (unlikely(!req)) {
		fput(file);
		return -EAGAIN;
	}
	req->ki_filp = file;
	if (iocb->aio_flags & IOCB_FLAG_RESFD) {
		 
		req->ki_eventfd = eventfd_ctx_fdget((int) iocb->aio_resfd);
		if (IS_ERR(req->ki_eventfd)) {
			ret = PTR_ERR(req->ki_eventfd);
			req->ki_eventfd = NULL;
			goto out_put_req;
		}
	}

	ret = put_user(req->ki_key, &user_iocb->aio_key);
	if (unlikely(ret)) {
		dprintk("EFAULT: aio_key\n");
		goto out_put_req;
	}

	req->ki_obj.user = user_iocb;
	req->ki_user_data = iocb->aio_data;
	req->ki_pos = iocb->aio_offset;

	req->ki_buf = (char __user *)(unsigned long)iocb->aio_buf;
	req->ki_left = req->ki_nbytes = iocb->aio_nbytes;
	req->ki_opcode = iocb->aio_lio_opcode;

	ret = aio_setup_iocb(req, compat);

	if (ret)
		goto out_put_req;

	spin_lock_irq(&ctx->ctx_lock);
	 
	if (ctx->dead) {
		spin_unlock_irq(&ctx->ctx_lock);
		ret = -EINVAL;
		goto out_put_req;
	}
	aio_run_iocb(req);
	if (!list_empty(&ctx->run_list)) {
		 
		while (__aio_run_iocbs(ctx))
			;
	}
	spin_unlock_irq(&ctx->ctx_lock);

	aio_put_req(req);	 
	return 0;

out_put_req:
	aio_put_req(req);	 
	aio_put_req(req);	 
	return ret;
}
