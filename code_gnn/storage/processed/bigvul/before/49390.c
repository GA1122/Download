static struct task_struct *first_tid(struct pid *pid, int tid, loff_t f_pos,
					struct pid_namespace *ns)
{
	struct task_struct *pos, *task;
	unsigned long nr = f_pos;

	if (nr != f_pos)	 
		return NULL;

	rcu_read_lock();
	task = pid_task(pid, PIDTYPE_PID);
	if (!task)
		goto fail;

	 
	if (tid && nr) {
		pos = find_task_by_pid_ns(tid, ns);
		if (pos && same_thread_group(pos, task))
			goto found;
	}

	 
	if (nr >= get_nr_threads(task))
		goto fail;

	 
	pos = task = task->group_leader;
	do {
		if (!nr--)
			goto found;
	} while_each_thread(task, pos);
fail:
	pos = NULL;
	goto out;
found:
	get_task_struct(pos);
out:
	rcu_read_unlock();
	return pos;
}
