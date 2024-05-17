static unsigned next_desc(struct vhost_virtqueue *vq, struct vring_desc *desc)
{
	unsigned int next;

	 
	if (!(desc->flags & cpu_to_vhost16(vq, VRING_DESC_F_NEXT)))
		return -1U;

	 
	next = vhost16_to_cpu(vq, desc->next);
	 
	 
	read_barrier_depends();

	return next;
}
