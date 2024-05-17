mem_cgroup_create(struct cgroup_subsys *ss, struct cgroup *cont)
{
	struct mem_cgroup *memcg, *parent;
	long error = -ENOMEM;
	int node;

	memcg = mem_cgroup_alloc();
	if (!memcg)
		return ERR_PTR(error);

	for_each_node(node)
		if (alloc_mem_cgroup_per_zone_info(memcg, node))
			goto free_out;

	 
	if (cont->parent == NULL) {
		int cpu;
		enable_swap_cgroup();
		parent = NULL;
		if (mem_cgroup_soft_limit_tree_init())
			goto free_out;
		root_mem_cgroup = memcg;
		for_each_possible_cpu(cpu) {
			struct memcg_stock_pcp *stock =
						&per_cpu(memcg_stock, cpu);
			INIT_WORK(&stock->work, drain_local_stock);
		}
		hotcpu_notifier(memcg_cpu_hotplug_callback, 0);
	} else {
		parent = mem_cgroup_from_cont(cont->parent);
		memcg->use_hierarchy = parent->use_hierarchy;
		memcg->oom_kill_disable = parent->oom_kill_disable;
	}

	if (parent && parent->use_hierarchy) {
		res_counter_init(&memcg->res, &parent->res);
		res_counter_init(&memcg->memsw, &parent->memsw);
		 
		mem_cgroup_get(parent);
	} else {
		res_counter_init(&memcg->res, NULL);
		res_counter_init(&memcg->memsw, NULL);
	}
	memcg->last_scanned_node = MAX_NUMNODES;
	INIT_LIST_HEAD(&memcg->oom_notify);

	if (parent)
		memcg->swappiness = mem_cgroup_swappiness(parent);
	atomic_set(&memcg->refcnt, 1);
	memcg->move_charge_at_immigrate = 0;
	mutex_init(&memcg->thresholds_lock);
	return &memcg->css;
free_out:
	__mem_cgroup_free(memcg);
	return ERR_PTR(error);
}