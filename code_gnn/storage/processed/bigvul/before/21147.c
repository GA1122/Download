static void mem_cgroup_update_tree(struct mem_cgroup *memcg, struct page *page)
{
	unsigned long long excess;
	struct mem_cgroup_per_zone *mz;
	struct mem_cgroup_tree_per_zone *mctz;
	int nid = page_to_nid(page);
	int zid = page_zonenum(page);
	mctz = soft_limit_tree_from_page(page);

	 
	for (; memcg; memcg = parent_mem_cgroup(memcg)) {
		mz = mem_cgroup_zoneinfo(memcg, nid, zid);
		excess = res_counter_soft_limit_excess(&memcg->res);
		 
		if (excess || mz->on_tree) {
			spin_lock(&mctz->lock);
			 
			if (mz->on_tree)
				__mem_cgroup_remove_exceeded(memcg, mz, mctz);
			 
			__mem_cgroup_insert_exceeded(memcg, mz, mctz, excess);
			spin_unlock(&mctz->lock);
		}
	}
}
