static void mem_cgroup_do_uncharge(struct mem_cgroup *memcg,
				   unsigned int nr_pages,
				   const enum charge_type ctype)
{
	struct memcg_batch_info *batch = NULL;
	bool uncharge_memsw = true;

	 
	if (!do_swap_account || ctype == MEM_CGROUP_CHARGE_TYPE_SWAPOUT)
		uncharge_memsw = false;

	batch = &current->memcg_batch;
	 
	if (!batch->memcg)
		batch->memcg = memcg;
	 

	if (!batch->do_batch || test_thread_flag(TIF_MEMDIE))
		goto direct_uncharge;

	if (nr_pages > 1)
		goto direct_uncharge;

	 
	if (batch->memcg != memcg)
		goto direct_uncharge;
	 
	batch->nr_pages++;
	if (uncharge_memsw)
		batch->memsw_nr_pages++;
	return;
direct_uncharge:
	res_counter_uncharge(&memcg->res, nr_pages * PAGE_SIZE);
	if (uncharge_memsw)
		res_counter_uncharge(&memcg->memsw, nr_pages * PAGE_SIZE);
	if (unlikely(batch->memcg != memcg))
		memcg_oom_recover(memcg);
	return;
}
