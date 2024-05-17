static void request_wait_answer(struct fuse_conn *fc, struct fuse_req *req)
{
	struct fuse_iqueue *fiq = &fc->iq;
	int err;

	if (!fc->no_interrupt) {
		 
		err = wait_event_interruptible(req->waitq,
					test_bit(FR_FINISHED, &req->flags));
		if (!err)
			return;

		set_bit(FR_INTERRUPTED, &req->flags);
		 
		smp_mb__after_atomic();
		if (test_bit(FR_SENT, &req->flags))
			queue_interrupt(fiq, req);
	}

	if (!test_bit(FR_FORCE, &req->flags)) {
		 
		err = wait_event_killable(req->waitq,
					test_bit(FR_FINISHED, &req->flags));
		if (!err)
			return;

		spin_lock(&fiq->waitq.lock);
		 
		if (test_bit(FR_PENDING, &req->flags)) {
			list_del(&req->list);
			spin_unlock(&fiq->waitq.lock);
			__fuse_put_request(req);
			req->out.h.error = -EINTR;
			return;
		}
		spin_unlock(&fiq->waitq.lock);
	}

	 
	wait_event(req->waitq, test_bit(FR_FINISHED, &req->flags));
}
