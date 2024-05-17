static void perf_cgroup_exit(struct cgroup_subsys *ss, struct cgroup *cgrp,
		struct cgroup *old_cgrp, struct task_struct *task)
{
	 
	if (!(task->flags & PF_EXITING))
		return;

	perf_cgroup_attach_task(cgrp, task);
}
