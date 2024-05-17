static unsigned next_desc(struct vring_desc *desc)
{
	unsigned int next;

	 
	if (!(desc->flags & VRING_DESC_F_NEXT))
		return -1U;

	 
	next = desc->next;
	 
	 
	read_barrier_depends();

	return next;
}
