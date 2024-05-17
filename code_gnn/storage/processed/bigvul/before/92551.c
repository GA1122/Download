static struct sched_group *find_busiest_group(struct lb_env *env)
{
	struct sg_lb_stats *local, *busiest;
	struct sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	 
	update_sd_lb_stats(env, &sds);

	if (static_branch_unlikely(&sched_energy_present)) {
		struct root_domain *rd = env->dst_rq->rd;

		if (rcu_dereference(rd->pd) && !READ_ONCE(rd->overutilized))
			goto out_balanced;
	}

	local = &sds.local_stat;
	busiest = &sds.busiest_stat;

	 
	if (check_asym_packing(env, &sds))
		return sds.busiest;

	 
	if (!sds.busiest || busiest->sum_nr_running == 0)
		goto out_balanced;

	 
	sds.avg_load = (SCHED_CAPACITY_SCALE * sds.total_load)
						/ sds.total_capacity;

	 
	if (busiest->group_type == group_imbalanced)
		goto force_balance;

	 
	if (env->idle != CPU_NOT_IDLE && group_has_capacity(env, local) &&
	    busiest->group_no_capacity)
		goto force_balance;

	 
	if (busiest->group_type == group_misfit_task)
		goto force_balance;

	 
	if (local->avg_load >= busiest->avg_load)
		goto out_balanced;

	 
	if (local->avg_load >= sds.avg_load)
		goto out_balanced;

	if (env->idle == CPU_IDLE) {
		 
		if ((busiest->group_type != group_overloaded) &&
				(local->idle_cpus <= (busiest->idle_cpus + 1)))
			goto out_balanced;
	} else {
		 
		if (100 * busiest->avg_load <=
				env->sd->imbalance_pct * local->avg_load)
			goto out_balanced;
	}

force_balance:
	 
	env->src_grp_type = busiest->group_type;
	calculate_imbalance(env, &sds);
	return env->imbalance ? sds.busiest : NULL;

out_balanced:
	env->imbalance = 0;
	return NULL;
}
