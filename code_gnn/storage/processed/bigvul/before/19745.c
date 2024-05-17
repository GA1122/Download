static long region_add(struct list_head *head, long f, long t)
{
	struct file_region *rg, *nrg, *trg;

	 
	list_for_each_entry(rg, head, link)
		if (f <= rg->to)
			break;

	 
	if (f > rg->from)
		f = rg->from;

	 
	nrg = rg;
	list_for_each_entry_safe(rg, trg, rg->link.prev, link) {
		if (&rg->link == head)
			break;
		if (rg->from > t)
			break;

		 
		if (rg->to > t)
			t = rg->to;
		if (rg != nrg) {
			list_del(&rg->link);
			kfree(rg);
		}
	}
	nrg->from = f;
	nrg->to = t;
	return 0;
}
