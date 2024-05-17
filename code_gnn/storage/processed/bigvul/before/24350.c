static int oom_kill_task(struct task_struct *p, struct mem_cgroup *mem)
{
	struct task_struct *q;
	struct mm_struct *mm;

	p = find_lock_task_mm(p);
	if (!p)
		return 1;

	 
	mm = p->mm;

	pr_err("Killed process %d (%s) total-vm:%lukB, anon-rss:%lukB, file-rss:%lukB\n",
		task_pid_nr(p), p->comm, K(p->mm->total_vm),
		K(get_mm_counter(p->mm, MM_ANONPAGES)),
		K(get_mm_counter(p->mm, MM_FILEPAGES)));
	task_unlock(p);

	 
	for_each_process(q)
		if (q->mm == mm && !same_thread_group(q, p)) {
			task_lock(q);	 
			pr_err("Kill process %d (%s) sharing same memory\n",
				task_pid_nr(q), q->comm);
			task_unlock(q);
			force_sig(SIGKILL, q);
		}

	set_tsk_thread_flag(p, TIF_MEMDIE);
	force_sig(SIGKILL, p);

	return 0;
}
