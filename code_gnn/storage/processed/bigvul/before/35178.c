int get_signal_to_deliver(siginfo_t *info, struct k_sigaction *return_ka,
			  struct pt_regs *regs, void *cookie)
{
	struct sighand_struct *sighand = current->sighand;
	struct signal_struct *signal = current->signal;
	int signr;

relock:
	 
	try_to_freeze();

	spin_lock_irq(&sighand->siglock);
	 
	if (unlikely(signal->flags & SIGNAL_CLD_MASK)) {
		int why = (signal->flags & SIGNAL_STOP_CONTINUED)
				? CLD_CONTINUED : CLD_STOPPED;
		signal->flags &= ~SIGNAL_CLD_MASK;

		why = tracehook_notify_jctl(why, CLD_CONTINUED);
		spin_unlock_irq(&sighand->siglock);

		if (why) {
			read_lock(&tasklist_lock);
			do_notify_parent_cldstop(current->group_leader, why);
			read_unlock(&tasklist_lock);
		}
		goto relock;
	}

	for (;;) {
		struct k_sigaction *ka;
		 
		signr = tracehook_get_signal(current, regs, info, return_ka);
		if (unlikely(signr < 0))
			goto relock;
		if (unlikely(signr != 0))
			ka = return_ka;
		else {
			if (unlikely(signal->group_stop_count > 0) &&
			    do_signal_stop(0))
				goto relock;

			signr = dequeue_signal(current, &current->blocked,
					       info);

			if (!signr)
				break;  

			if (signr != SIGKILL) {
				signr = ptrace_signal(signr, info,
						      regs, cookie);
				if (!signr)
					continue;
			}

			ka = &sighand->action[signr-1];
		}

		 
		trace_signal_deliver(signr, info, ka);

		if (ka->sa.sa_handler == SIG_IGN)  
			continue;
		if (ka->sa.sa_handler != SIG_DFL) {
			 
			*return_ka = *ka;

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

			if (likely(do_signal_stop(info->si_signo))) {
				 
				goto relock;
			}

			 
			continue;
		}

		spin_unlock_irq(&sighand->siglock);

		 
		current->flags |= PF_SIGNALED;

		if (sig_kernel_coredump(signr)) {
			if (print_fatal_signals)
				print_fatal_signal(regs, info->si_signo);
			 
			do_coredump(info->si_signo, info->si_signo, regs);
		}

		 
		do_group_exit(info->si_signo);
		 
	}
	spin_unlock_irq(&sighand->siglock);
	return signr;
}
