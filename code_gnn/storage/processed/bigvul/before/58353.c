asmlinkage int syscall_trace_enter(struct pt_regs *regs, int scno)
{
	current_thread_info()->syscall = scno;

	 
	if (secure_computing(scno) == -1)
		return -1;

	if (test_thread_flag(TIF_SYSCALL_TRACE))
		scno = tracehook_report_syscall(regs, PTRACE_SYSCALL_ENTER);

	if (test_thread_flag(TIF_SYSCALL_TRACEPOINT))
		trace_sys_enter(regs, scno);

	audit_syscall_entry(AUDIT_ARCH_ARM, scno, regs->ARM_r0, regs->ARM_r1,
			    regs->ARM_r2, regs->ARM_r3);

	return scno;
}