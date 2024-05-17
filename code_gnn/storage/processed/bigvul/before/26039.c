static inline void perf_cgroup_sched_in(struct task_struct *task)
{
	perf_cgroup_switch(task, PERF_CGROUP_SWIN);
}
