static int need_active_balance(struct lb_env *env)
{
	struct sched_domain *sd = env->sd;

	if (env->idle == CPU_NEWLY_IDLE) {

		 
		if ((sd->flags & SD_ASYM_PACKING) &&
		    sched_asym_prefer(env->dst_cpu, env->src_cpu))
			return 1;
	}

	 
	if ((env->idle != CPU_NOT_IDLE) &&
	    (env->src_rq->cfs.h_nr_running == 1)) {
		if ((check_cpu_capacity(env->src_rq, sd)) &&
		    (capacity_of(env->src_cpu)*sd->imbalance_pct < capacity_of(env->dst_cpu)*100))
			return 1;
	}

	if (env->src_grp_type == group_misfit_task)
		return 1;

	return unlikely(sd->nr_balance_failed > sd->cache_nice_tries+2);
}
