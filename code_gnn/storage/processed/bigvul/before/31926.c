static void perf_cgroup_exit(struct cgroup *cgrp, struct cgroup *old_cgrp,
			     struct task_struct *task)
{
	 
	if (!(task->flags & PF_EXITING))
		return;

	task_function_call(task, __perf_cgroup_move, task);
}
