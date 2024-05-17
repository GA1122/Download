void daemonize(const char *name, ...)
{
	va_list args;
	sigset_t blocked;

	va_start(args, name);
	vsnprintf(current->comm, sizeof(current->comm), name, args);
	va_end(args);

	 
	exit_mm(current);
	 
	current->flags |= (PF_NOFREEZE | PF_KTHREAD);

	if (current->nsproxy != &init_nsproxy) {
		get_nsproxy(&init_nsproxy);
		switch_task_namespaces(current, &init_nsproxy);
	}
	set_special_pids(&init_struct_pid);
	proc_clear_tty(current);

	 
	sigfillset(&blocked);
	sigprocmask(SIG_BLOCK, &blocked, NULL);
	flush_signals(current);

	 

	daemonize_fs_struct();
	exit_files(current);
	current->files = init_task.files;
	atomic_inc(&current->files->count);

	reparent_to_kthreadd();
}
