static struct rq *find_busiest_queue(struct lb_env *env,
				     struct sched_group *group)
{
	struct rq *busiest = NULL, *rq;
	unsigned long busiest_load = 0, busiest_capacity = 1;
	int i;

	for_each_cpu_and(i, sched_group_span(group), env->cpus) {
		unsigned long capacity, wl;
		enum fbq_type rt;

		rq = cpu_rq(i);
		rt = fbq_classify_rq(rq);

		 
		if (rt > env->fbq_type)
			continue;

		 
		if (env->src_grp_type == group_misfit_task) {
			if (rq->misfit_task_load > busiest_load) {
				busiest_load = rq->misfit_task_load;
				busiest = rq;
			}

			continue;
		}

		capacity = capacity_of(i);

		 
		if (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    capacity_of(env->dst_cpu) < capacity &&
		    rq->nr_running == 1)
			continue;

		wl = weighted_cpuload(rq);

		 

		if (rq->nr_running == 1 && wl > env->imbalance &&
		    !check_cpu_capacity(rq, env->sd))
			continue;

		 
		if (wl * busiest_capacity > busiest_load * capacity) {
			busiest_load = wl;
			busiest_capacity = capacity;
			busiest = rq;
		}
	}

	return busiest;
}
