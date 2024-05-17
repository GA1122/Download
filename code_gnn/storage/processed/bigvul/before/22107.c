static int do_wait_thread(struct task_struct *tsk, int *notask_error,
			  enum pid_type type, struct pid *pid, int options,
			  struct siginfo __user *infop, int __user *stat_addr,
			  struct rusage __user *ru)
{
	struct task_struct *p;

	list_for_each_entry(p, &tsk->children, sibling) {
		 
		if (!task_detached(p)) {
			int ret = wait_consider_task(tsk, 0, p, notask_error,
						     type, pid, options,
						     infop, stat_addr, ru);
			if (ret)
				return ret;
		}
	}

	return 0;
}