void mem_cgroup_uncharge_start(void)
{
	current->memcg_batch.do_batch++;
	 
	if (current->memcg_batch.do_batch == 1) {
		current->memcg_batch.memcg = NULL;
		current->memcg_batch.nr_pages = 0;
		current->memcg_batch.memsw_nr_pages = 0;
	}
}