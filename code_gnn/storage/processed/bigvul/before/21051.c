static int mem_cgroup_do_charge(struct mem_cgroup *memcg, gfp_t gfp_mask,
				unsigned int nr_pages, bool oom_check)
{
	unsigned long csize = nr_pages * PAGE_SIZE;
	struct mem_cgroup *mem_over_limit;
	struct res_counter *fail_res;
	unsigned long flags = 0;
	int ret;

	ret = res_counter_charge(&memcg->res, csize, &fail_res);

	if (likely(!ret)) {
		if (!do_swap_account)
			return CHARGE_OK;
		ret = res_counter_charge(&memcg->memsw, csize, &fail_res);
		if (likely(!ret))
			return CHARGE_OK;

		res_counter_uncharge(&memcg->res, csize);
		mem_over_limit = mem_cgroup_from_res_counter(fail_res, memsw);
		flags |= MEM_CGROUP_RECLAIM_NOSWAP;
	} else
		mem_over_limit = mem_cgroup_from_res_counter(fail_res, res);
	 
	if (nr_pages == CHARGE_BATCH)
		return CHARGE_RETRY;

	if (!(gfp_mask & __GFP_WAIT))
		return CHARGE_WOULDBLOCK;

	ret = mem_cgroup_reclaim(mem_over_limit, gfp_mask, flags);
	if (mem_cgroup_margin(mem_over_limit) >= nr_pages)
		return CHARGE_RETRY;
	 
	if (nr_pages == 1 && ret)
		return CHARGE_RETRY;

	 
	if (mem_cgroup_wait_acct_move(mem_over_limit))
		return CHARGE_RETRY;

	 
	if (!oom_check)
		return CHARGE_NOMEM;
	 
	if (!mem_cgroup_handle_oom(mem_over_limit, gfp_mask))
		return CHARGE_OOM_DIE;

	return CHARGE_RETRY;
}
