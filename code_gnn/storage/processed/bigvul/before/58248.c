static void wake_up_idle_cpu(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

	if (cpu == smp_processor_id())
		return;

	 
	if (rq->curr != rq->idle)
		return;

	 
	set_tsk_need_resched(rq->idle);

	 
	smp_mb();
	if (!tsk_is_polling(rq->idle))
		smp_send_reschedule(cpu);
}
