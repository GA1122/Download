void mem_cgroup_uncharge_end(void)
{
	struct memcg_batch_info *batch = &current->memcg_batch;

	if (!batch->do_batch)
		return;

	batch->do_batch--;
	if (batch->do_batch)  
		return;

	if (!batch->memcg)
		return;
	 
	if (batch->nr_pages)
		res_counter_uncharge(&batch->memcg->res,
				     batch->nr_pages * PAGE_SIZE);
	if (batch->memsw_nr_pages)
		res_counter_uncharge(&batch->memcg->memsw,
				     batch->memsw_nr_pages * PAGE_SIZE);
	memcg_oom_recover(batch->memcg);
	 
	batch->memcg = NULL;
}
