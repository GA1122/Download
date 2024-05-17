update_tg_cfs_runnable(struct cfs_rq *cfs_rq, struct sched_entity *se, struct cfs_rq *gcfs_rq)
{
	long delta_avg, running_sum, runnable_sum = gcfs_rq->prop_runnable_sum;
	unsigned long runnable_load_avg, load_avg;
	u64 runnable_load_sum, load_sum = 0;
	s64 delta_sum;

	if (!runnable_sum)
		return;

	gcfs_rq->prop_runnable_sum = 0;

	if (runnable_sum >= 0) {
		 
		runnable_sum += se->avg.load_sum;
		runnable_sum = min(runnable_sum, (long)LOAD_AVG_MAX);
	} else {
		 
		if (scale_load_down(gcfs_rq->load.weight)) {
			load_sum = div_s64(gcfs_rq->avg.load_sum,
				scale_load_down(gcfs_rq->load.weight));
		}

		 
		runnable_sum = min(se->avg.load_sum, load_sum);
	}

	 
	running_sum = se->avg.util_sum /
		arch_scale_cpu_capacity(NULL, cpu_of(rq_of(cfs_rq)));
	runnable_sum = max(runnable_sum, running_sum);

	load_sum = (s64)se_weight(se) * runnable_sum;
	load_avg = div_s64(load_sum, LOAD_AVG_MAX);

	delta_sum = load_sum - (s64)se_weight(se) * se->avg.load_sum;
	delta_avg = load_avg - se->avg.load_avg;

	se->avg.load_sum = runnable_sum;
	se->avg.load_avg = load_avg;
	add_positive(&cfs_rq->avg.load_avg, delta_avg);
	add_positive(&cfs_rq->avg.load_sum, delta_sum);

	runnable_load_sum = (s64)se_runnable(se) * runnable_sum;
	runnable_load_avg = div_s64(runnable_load_sum, LOAD_AVG_MAX);
	delta_sum = runnable_load_sum - se_weight(se) * se->avg.runnable_load_sum;
	delta_avg = runnable_load_avg - se->avg.runnable_load_avg;

	se->avg.runnable_load_sum = runnable_sum;
	se->avg.runnable_load_avg = runnable_load_avg;

	if (se->on_rq) {
		add_positive(&cfs_rq->avg.runnable_load_avg, delta_avg);
		add_positive(&cfs_rq->avg.runnable_load_sum, delta_sum);
	}
}
