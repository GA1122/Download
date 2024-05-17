static int mem_control_stat_show(struct cgroup *cont, struct cftype *cft,
				 struct cgroup_map_cb *cb)
{
	struct mem_cgroup *mem_cont = mem_cgroup_from_cont(cont);
	struct mcs_total_stat mystat;
	int i;

	memset(&mystat, 0, sizeof(mystat));
	mem_cgroup_get_local_stat(mem_cont, &mystat);


	for (i = 0; i < NR_MCS_STAT; i++) {
		if (i == MCS_SWAP && !do_swap_account)
			continue;
		cb->fill(cb, memcg_stat_strings[i].local_name, mystat.stat[i]);
	}

	 
	{
		unsigned long long limit, memsw_limit;
		memcg_get_hierarchical_limit(mem_cont, &limit, &memsw_limit);
		cb->fill(cb, "hierarchical_memory_limit", limit);
		if (do_swap_account)
			cb->fill(cb, "hierarchical_memsw_limit", memsw_limit);
	}

	memset(&mystat, 0, sizeof(mystat));
	mem_cgroup_get_total_stat(mem_cont, &mystat);
	for (i = 0; i < NR_MCS_STAT; i++) {
		if (i == MCS_SWAP && !do_swap_account)
			continue;
		cb->fill(cb, memcg_stat_strings[i].total_name, mystat.stat[i]);
	}

#ifdef CONFIG_DEBUG_VM
	{
		int nid, zid;
		struct mem_cgroup_per_zone *mz;
		unsigned long recent_rotated[2] = {0, 0};
		unsigned long recent_scanned[2] = {0, 0};

		for_each_online_node(nid)
			for (zid = 0; zid < MAX_NR_ZONES; zid++) {
				mz = mem_cgroup_zoneinfo(mem_cont, nid, zid);

				recent_rotated[0] +=
					mz->reclaim_stat.recent_rotated[0];
				recent_rotated[1] +=
					mz->reclaim_stat.recent_rotated[1];
				recent_scanned[0] +=
					mz->reclaim_stat.recent_scanned[0];
				recent_scanned[1] +=
					mz->reclaim_stat.recent_scanned[1];
			}
		cb->fill(cb, "recent_rotated_anon", recent_rotated[0]);
		cb->fill(cb, "recent_rotated_file", recent_rotated[1]);
		cb->fill(cb, "recent_scanned_anon", recent_scanned[0]);
		cb->fill(cb, "recent_scanned_file", recent_scanned[1]);
	}
#endif

	return 0;
}