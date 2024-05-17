quit_code(thread_t * thread, __attribute__((unused)) int status)
{
	smtp_t *smtp = THREAD_ARG(thread);

	 
	free_smtp_all(smtp);
	thread_close_fd(thread);
	return 0;
}
