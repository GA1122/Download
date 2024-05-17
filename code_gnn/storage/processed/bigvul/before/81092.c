static void arm_timer(struct k_itimer *timer)
{
	struct task_struct *p = timer->it.cpu.task;
	struct list_head *head, *listpos;
	struct task_cputime *cputime_expires;
	struct cpu_timer_list *const nt = &timer->it.cpu;
	struct cpu_timer_list *next;

	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		head = p->cpu_timers;
		cputime_expires = &p->cputime_expires;
	} else {
		head = p->signal->cpu_timers;
		cputime_expires = &p->signal->cputime_expires;
	}
	head += CPUCLOCK_WHICH(timer->it_clock);

	listpos = head;
	list_for_each_entry(next, head, entry) {
		if (nt->expires < next->expires)
			break;
		listpos = &next->entry;
	}
	list_add(&nt->entry, listpos);

	if (listpos == head) {
		u64 exp = nt->expires;

		 

		switch (CPUCLOCK_WHICH(timer->it_clock)) {
		case CPUCLOCK_PROF:
			if (expires_gt(cputime_expires->prof_exp, exp))
				cputime_expires->prof_exp = exp;
			break;
		case CPUCLOCK_VIRT:
			if (expires_gt(cputime_expires->virt_exp, exp))
				cputime_expires->virt_exp = exp;
			break;
		case CPUCLOCK_SCHED:
			if (expires_gt(cputime_expires->sched_exp, exp))
				cputime_expires->sched_exp = exp;
			break;
		}
		if (CPUCLOCK_PERTHREAD(timer->it_clock))
			tick_dep_set_task(p, TICK_DEP_BIT_POSIX_TIMER);
		else
			tick_dep_set_signal(p->signal, TICK_DEP_BIT_POSIX_TIMER);
	}
}