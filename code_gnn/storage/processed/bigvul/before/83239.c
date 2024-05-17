static void signal_delivered(struct ksignal *ksig, int stepping)
{
	sigset_t blocked;

	 
	clear_restore_sigmask();

	sigorsets(&blocked, &current->blocked, &ksig->ka.sa.sa_mask);
	if (!(ksig->ka.sa.sa_flags & SA_NODEFER))
		sigaddset(&blocked, ksig->sig);
	set_current_blocked(&blocked);
	tracehook_signal_handler(stepping);
}
