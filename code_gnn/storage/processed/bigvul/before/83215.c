static void collect_signal(int sig, struct sigpending *list, siginfo_t *info,
			   bool *resched_timer)
{
	struct sigqueue *q, *first = NULL;

	 
	list_for_each_entry(q, &list->list, list) {
		if (q->info.si_signo == sig) {
			if (first)
				goto still_pending;
			first = q;
		}
	}

	sigdelset(&list->signal, sig);

	if (first) {
still_pending:
		list_del_init(&first->list);
		copy_siginfo(info, &first->info);

		*resched_timer =
			(first->flags & SIGQUEUE_PREALLOC) &&
			(info->si_code == SI_TIMER) &&
			(info->si_sys_private);

		__sigqueue_free(first);
	} else {
		 
		info->si_signo = sig;
		info->si_errno = 0;
		info->si_code = SI_USER;
		info->si_pid = 0;
		info->si_uid = 0;
	}
}