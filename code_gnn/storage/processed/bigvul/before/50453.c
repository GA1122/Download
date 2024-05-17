static void perf_cgroup_exit(struct cgroup_subsys_state *css,
			     struct cgroup_subsys_state *old_css,
			     struct task_struct *task)
{
	 
	if (!(task->flags & PF_EXITING))
		return;

	task_function_call(task, __perf_cgroup_move, task);
}
