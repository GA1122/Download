static void ttwu_queue_remote(struct task_struct *p, int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	struct task_struct *next = rq->wake_list;

	for (;;) {
		struct task_struct *old = next;

		p->wake_entry = next;
		next = cmpxchg(&rq->wake_list, old, p);
		if (next == old)
			break;
	}

	if (!next)
		smp_send_reschedule(cpu);
}
