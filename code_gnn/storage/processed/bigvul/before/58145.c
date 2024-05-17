static void cpu_cgroup_exit(struct cgroup_subsys_state *css,
			    struct cgroup_subsys_state *old_css,
			    struct task_struct *task)
{
	 
	if (!(task->flags & PF_EXITING))
		return;

	sched_move_task(task);
}
