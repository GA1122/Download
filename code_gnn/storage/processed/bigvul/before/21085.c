static int mem_cgroup_move_account(struct page *page,
				   unsigned int nr_pages,
				   struct page_cgroup *pc,
				   struct mem_cgroup *from,
				   struct mem_cgroup *to,
				   bool uncharge)
{
	unsigned long flags;
	int ret;

	VM_BUG_ON(from == to);
	VM_BUG_ON(PageLRU(page));
	 
	ret = -EBUSY;
	if (nr_pages > 1 && !PageTransHuge(page))
		goto out;

	lock_page_cgroup(pc);

	ret = -EINVAL;
	if (!PageCgroupUsed(pc) || pc->mem_cgroup != from)
		goto unlock;

	move_lock_page_cgroup(pc, &flags);

	if (PageCgroupFileMapped(pc)) {
		 
		preempt_disable();
		__this_cpu_dec(from->stat->count[MEM_CGROUP_STAT_FILE_MAPPED]);
		__this_cpu_inc(to->stat->count[MEM_CGROUP_STAT_FILE_MAPPED]);
		preempt_enable();
	}
	mem_cgroup_charge_statistics(from, PageCgroupCache(pc), -nr_pages);
	if (uncharge)
		 
		__mem_cgroup_cancel_charge(from, nr_pages);

	 
	pc->mem_cgroup = to;
	mem_cgroup_charge_statistics(to, PageCgroupCache(pc), nr_pages);
	 
	move_unlock_page_cgroup(pc, &flags);
	ret = 0;
unlock:
	unlock_page_cgroup(pc);
	 
	memcg_check_events(to, page);
	memcg_check_events(from, page);
out:
	return ret;
}
