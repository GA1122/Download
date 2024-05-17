static void vhost_zerocopy_callback(struct ubuf_info *ubuf, bool success)
{
	struct vhost_net_ubuf_ref *ubufs = ubuf->ctx;
	struct vhost_virtqueue *vq = ubufs->vq;
	int cnt = atomic_read(&ubufs->kref.refcount);

	 
	if (cnt <= 2 || !(cnt % 16))
		vhost_poll_queue(&vq->poll);
	 
	vq->heads[ubuf->desc].len = success ?
		VHOST_DMA_DONE_LEN : VHOST_DMA_FAILED_LEN;
	vhost_net_ubuf_put(ubufs);
}
