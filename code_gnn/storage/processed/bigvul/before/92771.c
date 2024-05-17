static bool update_sd_pick_busiest(struct lb_env *env,
				   struct sd_lb_stats *sds,
				   struct sched_group *sg,
				   struct sg_lb_stats *sgs)
{
	struct sg_lb_stats *busiest = &sds->busiest_stat;

	 
	if (sgs->group_type == group_misfit_task &&
	    (!group_smaller_max_cpu_capacity(sg, sds->local) ||
	     !group_has_capacity(env, &sds->local_stat)))
		return false;

	if (sgs->group_type > busiest->group_type)
		return true;

	if (sgs->group_type < busiest->group_type)
		return false;

	if (sgs->avg_load <= busiest->avg_load)
		return false;

	if (!(env->sd->flags & SD_ASYM_CPUCAPACITY))
		goto asym_packing;

	 
	if (sgs->sum_nr_running <= sgs->group_weight &&
	    group_smaller_min_cpu_capacity(sds->local, sg))
		return false;

	 
	if (sgs->group_type == group_misfit_task &&
	    sgs->group_misfit_task_load < busiest->group_misfit_task_load)
		return false;

asym_packing:
	 
	if (!(env->sd->flags & SD_ASYM_PACKING))
		return true;

	 
	if (env->idle == CPU_NOT_IDLE)
		return true;
	 
	if (sgs->sum_nr_running &&
	    sched_asym_prefer(env->dst_cpu, sg->asym_prefer_cpu)) {
		if (!sds->busiest)
			return true;

		 
		if (sched_asym_prefer(sds->busiest->asym_prefer_cpu,
				      sg->asym_prefer_cpu))
			return true;
	}

	return false;
}
