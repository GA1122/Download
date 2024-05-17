__mem_cgroup_commit_charge_swapin(struct page *page, struct mem_cgroup *memcg,
					enum charge_type ctype)
{
	struct page_cgroup *pc;

	if (mem_cgroup_disabled())
		return;
	if (!memcg)
		return;
	cgroup_exclude_rmdir(&memcg->css);

	pc = lookup_page_cgroup(page);
	__mem_cgroup_commit_charge(memcg, page, 1, pc, ctype, true);
	 
	if (do_swap_account && PageSwapCache(page)) {
		swp_entry_t ent = {.val = page_private(page)};
		struct mem_cgroup *swap_memcg;
		unsigned short id;

		id = swap_cgroup_record(ent, 0);
		rcu_read_lock();
		swap_memcg = mem_cgroup_lookup(id);
		if (swap_memcg) {
			 
			if (!mem_cgroup_is_root(swap_memcg))
				res_counter_uncharge(&swap_memcg->memsw,
						     PAGE_SIZE);
			mem_cgroup_swap_statistics(swap_memcg, false);
			mem_cgroup_put(swap_memcg);
		}
		rcu_read_unlock();
	}
	 
	cgroup_release_and_wakeup_rmdir(&memcg->css);
}
