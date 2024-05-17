static int sig_handler_ignored(void __user *handler, int sig)
{
	 
	return handler == SIG_IGN ||
		(handler == SIG_DFL && sig_kernel_ignore(sig));
}
