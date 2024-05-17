 establish_child_signal_handlers (void)
 {
	struct sigaction dfl_action;

	 
	dfl_action.sa_handler = SIG_DFL;
	sigemptyset (&dfl_action.sa_mask);
	dfl_action.sa_flags = 0;
	
	sigaction (SIGINT, &dfl_action, NULL);
 	sigaction (SIGTERM, &dfl_action, NULL);
 }