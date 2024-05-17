static void cpu_load_update_nohz(struct rq *this_rq,
				 unsigned long curr_jiffies,
				 unsigned long load)
{
	unsigned long pending_updates;

	pending_updates = curr_jiffies - this_rq->last_load_update_tick;
	if (pending_updates) {
		this_rq->last_load_update_tick = curr_jiffies;
		 
		cpu_load_update(this_rq, load, pending_updates);
	}
}
