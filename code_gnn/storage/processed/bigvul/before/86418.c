static long region_chg(struct resv_map *resv, long f, long t)
{
	struct list_head *head = &resv->regions;
	struct file_region *rg, *nrg = NULL;
	long chg = 0;

retry:
	spin_lock(&resv->lock);
retry_locked:
	resv->adds_in_progress++;

	 
	if (resv->adds_in_progress > resv->region_cache_count) {
		struct file_region *trg;

		VM_BUG_ON(resv->adds_in_progress - resv->region_cache_count > 1);
		 
		resv->adds_in_progress--;
		spin_unlock(&resv->lock);

		trg = kmalloc(sizeof(*trg), GFP_KERNEL);
		if (!trg) {
			kfree(nrg);
			return -ENOMEM;
		}

		spin_lock(&resv->lock);
		list_add(&trg->link, &resv->region_cache);
		resv->region_cache_count++;
		goto retry_locked;
	}

	 
	list_for_each_entry(rg, head, link)
		if (f <= rg->to)
			break;

	 
	if (&rg->link == head || t < rg->from) {
		if (!nrg) {
			resv->adds_in_progress--;
			spin_unlock(&resv->lock);
			nrg = kmalloc(sizeof(*nrg), GFP_KERNEL);
			if (!nrg)
				return -ENOMEM;

			nrg->from = f;
			nrg->to   = f;
			INIT_LIST_HEAD(&nrg->link);
			goto retry;
		}

		list_add(&nrg->link, rg->link.prev);
		chg = t - f;
		goto out_nrg;
	}

	 
	if (f > rg->from)
		f = rg->from;
	chg = t - f;

	 
	list_for_each_entry(rg, rg->link.prev, link) {
		if (&rg->link == head)
			break;
		if (rg->from > t)
			goto out;

		 
		if (rg->to > t) {
			chg += rg->to - t;
			t = rg->to;
		}
		chg -= rg->to - rg->from;
	}

out:
	spin_unlock(&resv->lock);
	 
	kfree(nrg);
	return chg;
out_nrg:
	spin_unlock(&resv->lock);
	return chg;
}
