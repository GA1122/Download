static void install_handler(void) {
	struct sigaction sga;

	sigemptyset(&sga.sa_mask);
	sigaddset(&sga.sa_mask, SIGTERM);
	sga.sa_handler = sandbox_handler;
	sga.sa_flags = 0;
	sigaction(SIGINT, &sga, NULL);

	sigemptyset(&sga.sa_mask);
	sigaddset(&sga.sa_mask, SIGINT);
	sga.sa_handler = sandbox_handler;
	sga.sa_flags = 0;
	sigaction(SIGTERM, &sga, NULL);
}