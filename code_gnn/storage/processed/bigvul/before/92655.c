static __latent_entropy void run_rebalance_domains(struct softirq_action *h)
{
	struct rq *this_rq = this_rq();
	enum cpu_idle_type idle = this_rq->idle_balance ?
						CPU_IDLE : CPU_NOT_IDLE;

	 
	if (nohz_idle_balance(this_rq, idle))
		return;

	 
	update_blocked_averages(this_rq->cpu);
	rebalance_domains(this_rq, idle);
}
