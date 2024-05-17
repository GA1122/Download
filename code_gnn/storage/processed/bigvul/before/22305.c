static int __migrate_task(struct task_struct *p, int src_cpu, int dest_cpu)
{
	struct rq *rq_dest, *rq_src;
	int ret = 0;

	if (unlikely(!cpu_active(dest_cpu)))
		return ret;

	rq_src = cpu_rq(src_cpu);
	rq_dest = cpu_rq(dest_cpu);

	double_rq_lock(rq_src, rq_dest);
	 
	if (task_cpu(p) != src_cpu)
		goto done;
	 
	if (!cpumask_test_cpu(dest_cpu, &p->cpus_allowed))
		goto fail;

	 
	if (p->se.on_rq) {
		deactivate_task(rq_src, p, 0);
		set_task_cpu(p, dest_cpu);
		activate_task(rq_dest, p, 0);
		check_preempt_curr(rq_dest, p, 0);
	}
done:
	ret = 1;
fail:
	double_rq_unlock(rq_src, rq_dest);
	return ret;
}