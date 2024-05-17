cpu_cgroup_can_attach_task(struct cgroup *cgrp, struct task_struct *tsk)
{
#ifdef CONFIG_RT_GROUP_SCHED
	if (!sched_rt_can_attach(cgroup_tg(cgrp), tsk))
		return -EINVAL;
#else
	 
	if (tsk->sched_class != &fair_sched_class)
		return -EINVAL;
#endif
	return 0;
}