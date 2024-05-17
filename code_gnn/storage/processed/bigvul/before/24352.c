void out_of_memory(struct zonelist *zonelist, gfp_t gfp_mask,
		int order, nodemask_t *nodemask)
{
	const nodemask_t *mpol_mask;
	struct task_struct *p;
	unsigned long totalpages;
	unsigned long freed = 0;
	unsigned int points;
	enum oom_constraint constraint = CONSTRAINT_NONE;
	int killed = 0;

	blocking_notifier_call_chain(&oom_notify_list, 0, &freed);
	if (freed > 0)
		 
		return;

	 
	if (fatal_signal_pending(current)) {
		set_thread_flag(TIF_MEMDIE);
		return;
	}

	 
	constraint = constrained_alloc(zonelist, gfp_mask, nodemask,
						&totalpages);
	mpol_mask = (constraint == CONSTRAINT_MEMORY_POLICY) ? nodemask : NULL;
	check_panic_on_oom(constraint, gfp_mask, order, mpol_mask);

	read_lock(&tasklist_lock);
	if (sysctl_oom_kill_allocating_task &&
	    !oom_unkillable_task(current, NULL, nodemask) &&
	    current->mm && !atomic_read(&current->mm->oom_disable_count)) {
		 
		if (!oom_kill_process(current, gfp_mask, order, 0, totalpages,
				NULL, nodemask,
				"Out of memory (oom_kill_allocating_task)"))
			goto out;
	}

retry:
	p = select_bad_process(&points, totalpages, NULL, mpol_mask);
	if (PTR_ERR(p) == -1UL)
		goto out;

	 
	if (!p) {
		dump_header(NULL, gfp_mask, order, NULL, mpol_mask);
		read_unlock(&tasklist_lock);
		panic("Out of memory and no killable processes...\n");
	}

	if (oom_kill_process(p, gfp_mask, order, points, totalpages, NULL,
				nodemask, "Out of memory"))
		goto retry;
	killed = 1;
out:
	read_unlock(&tasklist_lock);

	 
	if (killed && !test_thread_flag(TIF_MEMDIE))
		schedule_timeout_uninterruptible(1);
}