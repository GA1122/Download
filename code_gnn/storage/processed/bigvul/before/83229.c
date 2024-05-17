int get_signal(struct ksignal *ksig)
{
	struct sighand_struct *sighand = current->sighand;
	struct signal_struct *signal = current->signal;
	int signr;

	if (unlikely(current->task_works))
		task_work_run();

	if (unlikely(uprobe_deny_signal()))
		return 0;

	 
	try_to_freeze();

relock:
	spin_lock_irq(&sighand->siglock);
	 
	if (unlikely(signal->flags & SIGNAL_CLD_MASK)) {
		int why;

		if (signal->flags & SIGNAL_CLD_CONTINUED)
			why = CLD_CONTINUED;
		else
			why = CLD_STOPPED;

		signal->flags &= ~SIGNAL_CLD_MASK;

		spin_unlock_irq(&sighand->siglock);

		 
		read_lock(&tasklist_lock);
		do_notify_parent_cldstop(current, false, why);

		if (ptrace_reparented(current->group_leader))
			do_notify_parent_cldstop(current->group_leader,
						true, why);
		read_unlock(&tasklist_lock);

		goto relock;
	}

	for (;;) {
		struct k_sigaction *ka;

		if (unlikely(current->jobctl & JOBCTL_STOP_PENDING) &&
		    do_signal_stop(0))
			goto relock;

		if (unlikely(current->jobctl & JOBCTL_TRAP_MASK)) {
			do_jobctl_trap();
			spin_unlock_irq(&sighand->siglock);
			goto relock;
		}

		signr = dequeue_signal(current, &current->blocked, &ksig->info);

		if (!signr)
			break;  

		if (unlikely(current->ptrace) && signr != SIGKILL) {
			signr = ptrace_signal(signr, &ksig->info);
			if (!signr)
				continue;
		}

		ka = &sighand->action[signr-1];

		 
		trace_signal_deliver(signr, &ksig->info, ka);

		if (ka->sa.sa_handler == SIG_IGN)  
			continue;
		if (ka->sa.sa_handler != SIG_DFL) {
			 
			ksig->ka = *ka;

			if (ka->sa.sa_flags & SA_ONESHOT)
				ka->sa.sa_handler = SIG_DFL;

			break;  
		}

		 
		if (sig_kernel_ignore(signr))  
			continue;

		 
		if (unlikely(signal->flags & SIGNAL_UNKILLABLE) &&
				!sig_kernel_only(signr))
			continue;

		if (sig_kernel_stop(signr)) {
			 
			if (signr != SIGSTOP) {
				spin_unlock_irq(&sighand->siglock);

				 

				if (is_current_pgrp_orphaned())
					goto relock;

				spin_lock_irq(&sighand->siglock);
			}

			if (likely(do_signal_stop(ksig->info.si_signo))) {
				 
				goto relock;
			}

			 
			continue;
		}

		spin_unlock_irq(&sighand->siglock);

		 
		current->flags |= PF_SIGNALED;

		if (sig_kernel_coredump(signr)) {
			if (print_fatal_signals)
				print_fatal_signal(ksig->info.si_signo);
			proc_coredump_connector(current);
			 
			do_coredump(&ksig->info);
		}

		 
		do_group_exit(ksig->info.si_signo);
		 
	}
	spin_unlock_irq(&sighand->siglock);

	ksig->sig = signr;
	return ksig->sig > 0;
}
