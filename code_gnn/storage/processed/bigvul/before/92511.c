static void cpu_load_update_idle(struct rq *this_rq)
{
	 
	if (weighted_cpuload(this_rq))
		return;

	cpu_load_update_nohz(this_rq, READ_ONCE(jiffies), 0);
}
