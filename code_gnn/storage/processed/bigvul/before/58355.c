static int tracehook_report_syscall(struct pt_regs *regs,
				    enum ptrace_syscall_dir dir)
{
	unsigned long ip;

	 
	ip = regs->ARM_ip;
	regs->ARM_ip = dir;

	if (dir == PTRACE_SYSCALL_EXIT)
		tracehook_report_syscall_exit(regs, 0);
	else if (tracehook_report_syscall_entry(regs))
		current_thread_info()->syscall = -1;

	regs->ARM_ip = ip;
	return current_thread_info()->syscall;
}
