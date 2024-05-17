static void __migrate_swap_task(struct task_struct *p, int cpu)
{
	if (p->on_rq) {
		struct rq *src_rq, *dst_rq;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		deactivate_task(src_rq, p, 0);
		set_task_cpu(p, cpu);
		activate_task(dst_rq, p, 0);
		check_preempt_curr(dst_rq, p, 0);
	} else {
		 
		p->wake_cpu = cpu;
	}
}
