cpu_cgroup_attach_task(struct cgroup *cgrp, struct task_struct *tsk)
{
	sched_move_task(tsk);
}
