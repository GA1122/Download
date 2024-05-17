static void nohz_newidle_balance(struct rq *this_rq)
{
	int this_cpu = this_rq->cpu;

	 
	if (!housekeeping_cpu(this_cpu, HK_FLAG_SCHED))
		return;

	 
	if (this_rq->avg_idle < sysctl_sched_migration_cost)
		return;

	 
	if (!READ_ONCE(nohz.has_blocked) ||
	    time_before(jiffies, READ_ONCE(nohz.next_blocked)))
		return;

	raw_spin_unlock(&this_rq->lock);
	 
	if (!_nohz_idle_balance(this_rq, NOHZ_STATS_KICK, CPU_NEWLY_IDLE))
		kick_ilb(NOHZ_STATS_KICK);
	raw_spin_lock(&this_rq->lock);
}
