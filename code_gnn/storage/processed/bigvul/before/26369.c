static void update_rq_clock_task(struct rq *rq, s64 delta)
{
	s64 irq_delta;

	irq_delta = irq_time_read(cpu_of(rq)) - rq->prev_irq_time;

	 
	if (irq_delta > delta)
		irq_delta = delta;

	rq->prev_irq_time += irq_delta;
	delta -= irq_delta;
	rq->clock_task += delta;

	if (irq_delta && sched_feat(NONIRQ_POWER))
		sched_rt_avg_update(rq, irq_delta);
}
