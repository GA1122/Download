static int oom_evaluate_task(struct task_struct *task, void *arg)
{
	struct oom_control *oc = arg;
	unsigned long points;

	if (oom_unkillable_task(task, NULL, oc->nodemask))
		goto next;

	 
	if (!is_sysrq_oom(oc) && tsk_is_oom_victim(task)) {
		if (test_bit(MMF_OOM_SKIP, &task->signal->oom_mm->flags))
			goto next;
		goto abort;
	}

	 
	if (oom_task_origin(task)) {
		points = ULONG_MAX;
		goto select;
	}

	points = oom_badness(task, NULL, oc->nodemask, oc->totalpages);
	if (!points || points < oc->chosen_points)
		goto next;

	 
	if (points == oc->chosen_points && thread_group_leader(oc->chosen))
		goto next;
select:
	if (oc->chosen)
		put_task_struct(oc->chosen);
	get_task_struct(task);
	oc->chosen = task;
	oc->chosen_points = points;
next:
	return 0;
abort:
	if (oc->chosen)
		put_task_struct(oc->chosen);
	oc->chosen = (void *)-1UL;
	return 1;
}
