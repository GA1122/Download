static int queue_interrupt(struct fuse_iqueue *fiq, struct fuse_req *req)
{
	spin_lock(&fiq->waitq.lock);
	 
	if (unlikely(!test_bit(FR_INTERRUPTED, &req->flags))) {
		spin_unlock(&fiq->waitq.lock);
		return -EINVAL;
	}

	if (list_empty(&req->intr_entry)) {
		list_add_tail(&req->intr_entry, &fiq->interrupts);
		 
		smp_mb();
		if (test_bit(FR_FINISHED, &req->flags)) {
			list_del_init(&req->intr_entry);
			spin_unlock(&fiq->waitq.lock);
			return 0;
		}
		wake_up_locked(&fiq->waitq);
		kill_fasync(&fiq->fasync, SIGIO, POLL_IN);
	}
	spin_unlock(&fiq->waitq.lock);
	return 0;
}
