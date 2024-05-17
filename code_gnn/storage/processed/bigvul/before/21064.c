u64 mem_cgroup_get_limit(struct mem_cgroup *memcg)
{
	u64 limit;
	u64 memsw;

	limit = res_counter_read_u64(&memcg->res, RES_LIMIT);
	limit += total_swap_pages << PAGE_SHIFT;

	memsw = res_counter_read_u64(&memcg->memsw, RES_LIMIT);
	 
	return min(limit, memsw);
}