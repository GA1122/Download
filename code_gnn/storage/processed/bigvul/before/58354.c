asmlinkage void syscall_trace_exit(struct pt_regs *regs)
{
	 
	audit_syscall_exit(regs);

	 
	if (test_thread_flag(TIF_SYSCALL_TRACEPOINT))
		trace_sys_exit(regs, regs_return_value(regs));

	if (test_thread_flag(TIF_SYSCALL_TRACE))
		tracehook_report_syscall(regs, PTRACE_SYSCALL_EXIT);
}
