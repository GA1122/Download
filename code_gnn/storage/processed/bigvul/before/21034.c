static struct mem_cgroup *mem_cgroup_alloc(void)
{
	struct mem_cgroup *mem;
	int size = sizeof(struct mem_cgroup);

	 
	if (size < PAGE_SIZE)
		mem = kzalloc(size, GFP_KERNEL);
	else
		mem = vzalloc(size);

	if (!mem)
		return NULL;

	mem->stat = alloc_percpu(struct mem_cgroup_stat_cpu);
	if (!mem->stat)
		goto out_free;
	spin_lock_init(&mem->pcp_counter_lock);
	return mem;

out_free:
	if (size < PAGE_SIZE)
		kfree(mem);
	else
		vfree(mem);
	return NULL;
}