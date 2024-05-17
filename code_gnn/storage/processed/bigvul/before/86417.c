static long region_add(struct resv_map *resv, long f, long t)
{
	struct list_head *head = &resv->regions;
	struct file_region *rg, *nrg, *trg;
	long add = 0;

	spin_lock(&resv->lock);
	 
	list_for_each_entry(rg, head, link)
		if (f <= rg->to)
			break;

	 
	if (&rg->link == head || t < rg->from) {
		VM_BUG_ON(resv->region_cache_count <= 0);

		resv->region_cache_count--;
		nrg = list_first_entry(&resv->region_cache, struct file_region,
					link);
		list_del(&nrg->link);

		nrg->from = f;
		nrg->to = t;
		list_add(&nrg->link, rg->link.prev);

		add += t - f;
		goto out_locked;
	}

	 
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
			 
			add -= (rg->to - rg->from);
			list_del(&rg->link);
			kfree(rg);
		}
	}

	add += (nrg->from - f);		 
	nrg->from = f;
	add += t - nrg->to;		 
	nrg->to = t;

out_locked:
	resv->adds_in_progress--;
	spin_unlock(&resv->lock);
	VM_BUG_ON(add < 0);
	return add;
}
