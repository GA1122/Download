static struct fuse_req *__fuse_get_req(struct fuse_conn *fc, unsigned npages,
				       bool for_background)
{
	struct fuse_req *req;
	int err;
	atomic_inc(&fc->num_waiting);

	if (fuse_block_alloc(fc, for_background)) {
		err = -EINTR;
		if (wait_event_killable_exclusive(fc->blocked_waitq,
				!fuse_block_alloc(fc, for_background)))
			goto out;
	}
	 
	smp_rmb();

	err = -ENOTCONN;
	if (!fc->connected)
		goto out;

	err = -ECONNREFUSED;
	if (fc->conn_error)
		goto out;

	req = fuse_request_alloc(npages);
	err = -ENOMEM;
	if (!req) {
		if (for_background)
			wake_up(&fc->blocked_waitq);
		goto out;
	}

	req->in.h.uid = from_kuid(fc->user_ns, current_fsuid());
	req->in.h.gid = from_kgid(fc->user_ns, current_fsgid());
	req->in.h.pid = pid_nr_ns(task_pid(current), fc->pid_ns);

	__set_bit(FR_WAITING, &req->flags);
	if (for_background)
		__set_bit(FR_BACKGROUND, &req->flags);

	if (unlikely(req->in.h.uid == ((uid_t)-1) ||
		     req->in.h.gid == ((gid_t)-1))) {
		fuse_put_request(fc, req);
		return ERR_PTR(-EOVERFLOW);
	}
	return req;

 out:
	fuse_drop_waiting(fc);
	return ERR_PTR(err);
}