void signal_delivered(int sig, siginfo_t *info, struct k_sigaction *ka,
			struct pt_regs *regs, int stepping)
{
	sigset_t blocked;

	 
	clear_restore_sigmask();

	sigorsets(&blocked, &current->blocked, &ka->sa.sa_mask);
	if (!(ka->sa.sa_flags & SA_NODEFER))
		sigaddset(&blocked, sig);
	set_current_blocked(&blocked);
	tracehook_signal_handler(sig, info, ka, regs, stepping);
}
