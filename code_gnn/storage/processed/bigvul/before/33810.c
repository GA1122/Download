static int vhost_update_avail_event(struct vhost_virtqueue *vq, u16 avail_event)
{
	if (__put_user(vq->avail_idx, vhost_avail_event(vq)))
		return -EFAULT;
	if (unlikely(vq->log_used)) {
		void __user *used;
		 
		smp_wmb();
		 
		used = vhost_avail_event(vq);
		log_write(vq->log_base, vq->log_addr +
			  (used - (void __user *)vq->used),
			  sizeof *vhost_avail_event(vq));
		if (vq->log_ctx)
			eventfd_signal(vq->log_ctx, 1);
	}
	return 0;
}