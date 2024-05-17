static int mem_cgroup_force_empty(struct mem_cgroup *memcg, bool free_all)
{
	int ret;
	int node, zid, shrink;
	int nr_retries = MEM_CGROUP_RECLAIM_RETRIES;
	struct cgroup *cgrp = memcg->css.cgroup;

	css_get(&memcg->css);

	shrink = 0;
	 
	if (free_all)
		goto try_to_free;
move_account:
	do {
		ret = -EBUSY;
		if (cgroup_task_count(cgrp) || !list_empty(&cgrp->children))
			goto out;
		ret = -EINTR;
		if (signal_pending(current))
			goto out;
		 
		lru_add_drain_all();
		drain_all_stock_sync(memcg);
		ret = 0;
		mem_cgroup_start_move(memcg);
		for_each_node_state(node, N_HIGH_MEMORY) {
			for (zid = 0; !ret && zid < MAX_NR_ZONES; zid++) {
				enum lru_list l;
				for_each_lru(l) {
					ret = mem_cgroup_force_empty_list(memcg,
							node, zid, l);
					if (ret)
						break;
				}
			}
			if (ret)
				break;
		}
		mem_cgroup_end_move(memcg);
		memcg_oom_recover(memcg);
		 
		if (ret == -ENOMEM)
			goto try_to_free;
		cond_resched();
	 
	} while (memcg->res.usage > 0 || ret);
out:
	css_put(&memcg->css);
	return ret;

try_to_free:
	 
	if (cgroup_task_count(cgrp) || !list_empty(&cgrp->children) || shrink) {
		ret = -EBUSY;
		goto out;
	}
	 
	lru_add_drain_all();
	 
	shrink = 1;
	while (nr_retries && memcg->res.usage > 0) {
		int progress;

		if (signal_pending(current)) {
			ret = -EINTR;
			goto out;
		}
		progress = try_to_free_mem_cgroup_pages(memcg, GFP_KERNEL,
						false);
		if (!progress) {
			nr_retries--;
			 
			congestion_wait(BLK_RW_ASYNC, HZ/10);
		}

	}
	lru_add_drain();
	 
	goto move_account;
}
