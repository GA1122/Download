static int sched_dl_global_constraints(void)
{
	u64 runtime = global_rt_runtime();
	u64 period = global_rt_period();
	u64 new_bw = to_ratio(period, runtime);
	int cpu, ret = 0;
	unsigned long flags;

	 
	for_each_possible_cpu(cpu) {
		struct dl_bw *dl_b = dl_bw_of(cpu);

		raw_spin_lock_irqsave(&dl_b->lock, flags);
		if (new_bw < dl_b->total_bw)
			ret = -EBUSY;
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);

		if (ret)
			break;
	}

	return ret;
}
