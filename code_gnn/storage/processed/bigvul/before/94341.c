static int wait_task_zombie(struct wait_opts *wo, struct task_struct *p)
{
	unsigned long state;
	int retval, status, traced;
	pid_t pid = task_pid_vnr(p);
	uid_t uid = __task_cred(p)->uid;
	struct siginfo __user *infop;

	if (!likely(wo->wo_flags & WEXITED))
		return 0;

	if (unlikely(wo->wo_flags & WNOWAIT)) {
		int exit_code = p->exit_code;
		int why, status;

		get_task_struct(p);
		read_unlock(&tasklist_lock);
		if ((exit_code & 0x7f) == 0) {
			why = CLD_EXITED;
			status = exit_code >> 8;
		} else {
			why = (exit_code & 0x80) ? CLD_DUMPED : CLD_KILLED;
			status = exit_code & 0x7f;
		}
		return wait_noreap_copyout(wo, p, pid, uid, why, status);
	}

	 
	state = xchg(&p->exit_state, EXIT_DEAD);
	if (state != EXIT_ZOMBIE) {
		BUG_ON(state != EXIT_DEAD);
		return 0;
	}

	traced = ptrace_reparented(p);
	 
	if (likely(!traced) && likely(!task_detached(p))) {
		struct signal_struct *psig;
		struct signal_struct *sig;
		unsigned long maxrss;

		 
		spin_lock_irq(&p->real_parent->sighand->siglock);
		psig = p->real_parent->signal;
		sig = p->signal;
		psig->cutime =
			cputime_add(psig->cutime,
			cputime_add(p->utime,
			cputime_add(sig->utime,
				    sig->cutime)));
		psig->cstime =
			cputime_add(psig->cstime,
			cputime_add(p->stime,
			cputime_add(sig->stime,
				    sig->cstime)));
		psig->cgtime =
			cputime_add(psig->cgtime,
			cputime_add(p->gtime,
			cputime_add(sig->gtime,
				    sig->cgtime)));
		psig->cmin_flt +=
			p->min_flt + sig->min_flt + sig->cmin_flt;
		psig->cmaj_flt +=
			p->maj_flt + sig->maj_flt + sig->cmaj_flt;
		psig->cnvcsw +=
			p->nvcsw + sig->nvcsw + sig->cnvcsw;
		psig->cnivcsw +=
			p->nivcsw + sig->nivcsw + sig->cnivcsw;
		psig->cinblock +=
			task_io_get_inblock(p) +
			sig->inblock + sig->cinblock;
		psig->coublock +=
			task_io_get_oublock(p) +
			sig->oublock + sig->coublock;
		maxrss = max(sig->maxrss, sig->cmaxrss);
		if (psig->cmaxrss < maxrss)
			psig->cmaxrss = maxrss;
		task_io_accounting_add(&psig->ioac, &p->ioac);
		task_io_accounting_add(&psig->ioac, &sig->ioac);
		spin_unlock_irq(&p->real_parent->sighand->siglock);
	}

	 
	read_unlock(&tasklist_lock);

	retval = wo->wo_rusage
		? getrusage(p, RUSAGE_BOTH, wo->wo_rusage) : 0;
	status = (p->signal->flags & SIGNAL_GROUP_EXIT)
		? p->signal->group_exit_code : p->exit_code;
	if (!retval && wo->wo_stat)
		retval = put_user(status, wo->wo_stat);

	infop = wo->wo_info;
	if (!retval && infop)
		retval = put_user(SIGCHLD, &infop->si_signo);
	if (!retval && infop)
		retval = put_user(0, &infop->si_errno);
	if (!retval && infop) {
		int why;

		if ((status & 0x7f) == 0) {
			why = CLD_EXITED;
			status >>= 8;
		} else {
			why = (status & 0x80) ? CLD_DUMPED : CLD_KILLED;
			status &= 0x7f;
		}
		retval = put_user((short)why, &infop->si_code);
		if (!retval)
			retval = put_user(status, &infop->si_status);
	}
	if (!retval && infop)
		retval = put_user(pid, &infop->si_pid);
	if (!retval && infop)
		retval = put_user(uid, &infop->si_uid);
	if (!retval)
		retval = pid;

	if (traced) {
		write_lock_irq(&tasklist_lock);
		 
		ptrace_unlink(p);
		 
		if (!task_detached(p)) {
			do_notify_parent(p, p->exit_signal);
			if (!task_detached(p)) {
				p->exit_state = EXIT_ZOMBIE;
				p = NULL;
			}
		}
		write_unlock_irq(&tasklist_lock);
	}
	if (p != NULL)
		release_task(p);

	return retval;
}