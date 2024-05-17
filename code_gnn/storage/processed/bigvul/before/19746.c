static long region_chg(struct list_head *head, long f, long t)
{
	struct file_region *rg, *nrg;
	long chg = 0;

	 
	list_for_each_entry(rg, head, link)
		if (f <= rg->to)
			break;

	 
	if (&rg->link == head || t < rg->from) {
		nrg = kmalloc(sizeof(*nrg), GFP_KERNEL);
		if (!nrg)
			return -ENOMEM;
		nrg->from = f;
		nrg->to   = f;
		INIT_LIST_HEAD(&nrg->link);
		list_add(&nrg->link, rg->link.prev);

		return t - f;
	}

	 
	if (f > rg->from)
		f = rg->from;
	chg = t - f;

	 
	list_for_each_entry(rg, rg->link.prev, link) {
		if (&rg->link == head)
			break;
		if (rg->from > t)
			return chg;

		 
		if (rg->to > t) {
			chg += rg->to - t;
			t = rg->to;
		}
		chg -= rg->to - rg->from;
	}
	return chg;
}
