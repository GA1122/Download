static long region_truncate(struct list_head *head, long end)
{
	struct file_region *rg, *trg;
	long chg = 0;

	 
	list_for_each_entry(rg, head, link)
		if (end <= rg->to)
			break;
	if (&rg->link == head)
		return 0;

	 
	if (end > rg->from) {
		chg = rg->to - end;
		rg->to = end;
		rg = list_entry(rg->link.next, typeof(*rg), link);
	}

	 
	list_for_each_entry_safe(rg, trg, rg->link.prev, link) {
		if (&rg->link == head)
			break;
		chg += rg->to - rg->from;
		list_del(&rg->link);
		kfree(rg);
	}
	return chg;
}