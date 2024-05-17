static struct task_struct *select_bad_process(unsigned int *ppoints,
		unsigned long totalpages, struct mem_cgroup *mem,
		const nodemask_t *nodemask)
{
	struct task_struct *g, *p;
	struct task_struct *chosen = NULL;
	*ppoints = 0;

	do_each_thread(g, p) {
		unsigned int points;

		if (p->exit_state)
			continue;
		if (oom_unkillable_task(p, mem, nodemask))
			continue;

		 
		if (test_tsk_thread_flag(p, TIF_MEMDIE))
			return ERR_PTR(-1UL);
		if (!p->mm)
			continue;

		if (p->flags & PF_EXITING) {
			 
			if (p == current) {
				chosen = p;
				*ppoints = 1000;
			} else {
				 
				if (!(p->group_leader->ptrace & PT_TRACE_EXIT))
					return ERR_PTR(-1UL);
			}
		}

		points = oom_badness(p, mem, nodemask, totalpages);
		if (points > *ppoints) {
			chosen = p;
			*ppoints = points;
		}
	} while_each_thread(g, p);

	return chosen;
}
