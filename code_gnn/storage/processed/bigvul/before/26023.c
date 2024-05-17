perf_cgroup_attach_task(struct cgroup *cgrp, struct task_struct *task)
{
	task_function_call(task, __perf_cgroup_move, task);
}
