static void sched_irq_time_avg_update(struct rq *rq, u64 curr_irq_time)
{
	if (sched_clock_irqtime && sched_feat(NONIRQ_POWER)) {
		u64 delta_irq = curr_irq_time - rq->prev_irq_time;
		rq->prev_irq_time = curr_irq_time;
		sched_rt_avg_update(rq, delta_irq);
	}
}
