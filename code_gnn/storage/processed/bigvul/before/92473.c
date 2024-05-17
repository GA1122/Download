static inline void calculate_imbalance(struct lb_env *env, struct sd_lb_stats *sds)
{
	unsigned long max_pull, load_above_capacity = ~0UL;
	struct sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	if (busiest->group_type == group_imbalanced) {
		 
		busiest->load_per_task =
			min(busiest->load_per_task, sds->avg_load);
	}

	 
	if (busiest->group_type != group_misfit_task &&
	    (busiest->avg_load <= sds->avg_load ||
	     local->avg_load >= sds->avg_load)) {
		env->imbalance = 0;
		return fix_small_imbalance(env, sds);
	}

	 
	if (busiest->group_type == group_overloaded &&
	    local->group_type   == group_overloaded) {
		load_above_capacity = busiest->sum_nr_running * SCHED_CAPACITY_SCALE;
		if (load_above_capacity > busiest->group_capacity) {
			load_above_capacity -= busiest->group_capacity;
			load_above_capacity *= scale_load_down(NICE_0_LOAD);
			load_above_capacity /= busiest->group_capacity;
		} else
			load_above_capacity = ~0UL;
	}

	 
	max_pull = min(busiest->avg_load - sds->avg_load, load_above_capacity);

	 
	env->imbalance = min(
		max_pull * busiest->group_capacity,
		(sds->avg_load - local->avg_load) * local->group_capacity
	) / SCHED_CAPACITY_SCALE;

	 
	if (busiest->group_type == group_misfit_task) {
		env->imbalance = max_t(long, env->imbalance,
				       busiest->group_misfit_task_load);
	}

	 
	if (env->imbalance < busiest->load_per_task)
		return fix_small_imbalance(env, sds);
}
