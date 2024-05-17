static struct swap_info_struct *alloc_swap_info(void)
{
	struct swap_info_struct *p;
	unsigned int type;

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (!p)
		return ERR_PTR(-ENOMEM);

	spin_lock(&swap_lock);
	for (type = 0; type < nr_swapfiles; type++) {
		if (!(swap_info[type]->flags & SWP_USED))
			break;
	}
	if (type >= MAX_SWAPFILES) {
		spin_unlock(&swap_lock);
		kfree(p);
		return ERR_PTR(-EPERM);
	}
	if (type >= nr_swapfiles) {
		p->type = type;
		swap_info[type] = p;
		 
		smp_wmb();
		nr_swapfiles++;
	} else {
		kfree(p);
		p = swap_info[type];
		 
	}
	INIT_LIST_HEAD(&p->first_swap_extent.list);
	p->flags = SWP_USED;
	p->next = -1;
	spin_unlock(&swap_lock);

	return p;
}
