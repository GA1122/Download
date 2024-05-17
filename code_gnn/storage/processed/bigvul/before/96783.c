static void __fuse_request_send(struct fuse_conn *fc, struct fuse_req *req)
{
	struct fuse_iqueue *fiq = &fc->iq;

	BUG_ON(test_bit(FR_BACKGROUND, &req->flags));
	spin_lock(&fiq->waitq.lock);
	if (!fiq->connected) {
		spin_unlock(&fiq->waitq.lock);
		req->out.h.error = -ENOTCONN;
	} else {
		req->in.h.unique = fuse_get_unique(fiq);
		queue_request(fiq, req);
		 
		__fuse_get_request(req);
		spin_unlock(&fiq->waitq.lock);

		request_wait_answer(fc, req);
		 
		smp_rmb();
	}
}
