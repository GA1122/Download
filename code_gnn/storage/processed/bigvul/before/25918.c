void syscall_trace_leave(struct pt_regs *regs)
{
	bool step;

	if (unlikely(current->audit_context))
		audit_syscall_exit(AUDITSC_RESULT(regs->ax), regs->ax);

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_exit(regs, regs->ax);

	 
	step = unlikely(test_thread_flag(TIF_SINGLESTEP)) &&
			!test_thread_flag(TIF_SYSCALL_EMU);
	if (step || test_thread_flag(TIF_SYSCALL_TRACE))
		tracehook_report_syscall_exit(regs, step);
}
