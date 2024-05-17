static int mem_cgroup_move_parent(struct page *page,
				  struct page_cgroup *pc,
				  struct mem_cgroup *child,
				  gfp_t gfp_mask)
{
	struct cgroup *cg = child->css.cgroup;
	struct cgroup *pcg = cg->parent;
	struct mem_cgroup *parent;
	unsigned int nr_pages;
	unsigned long uninitialized_var(flags);
	int ret;

	 
	if (!pcg)
		return -EINVAL;

	ret = -EBUSY;
	if (!get_page_unless_zero(page))
		goto out;
	if (isolate_lru_page(page))
		goto put;

	nr_pages = hpage_nr_pages(page);

	parent = mem_cgroup_from_cont(pcg);
	ret = __mem_cgroup_try_charge(NULL, gfp_mask, nr_pages, &parent, false);
	if (ret)
		goto put_back;

	if (nr_pages > 1)
		flags = compound_lock_irqsave(page);

	ret = mem_cgroup_move_account(page, nr_pages, pc, child, parent, true);
	if (ret)
		__mem_cgroup_cancel_charge(parent, nr_pages);

	if (nr_pages > 1)
		compound_unlock_irqrestore(page, flags);
put_back:
	putback_lru_page(page);
put:
	put_page(page);
out:
	return ret;
}