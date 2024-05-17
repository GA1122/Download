void cpu_load_update_nohz_start(void)
{
	struct rq *this_rq = this_rq();

	 
	this_rq->cpu_load[0] = weighted_cpuload(this_rq);
}
