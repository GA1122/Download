asmlinkage long do_syscall_trace_enter(struct pt_regs *regs)
{
	long ret = 0;

	secure_computing(regs->regs[0]);

	if (test_thread_flag(TIF_SYSCALL_TRACE) &&
	    tracehook_report_syscall_entry(regs))
		 
		ret = -1L;

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_enter(regs, regs->regs[0]);

	if (unlikely(current->audit_context))
		audit_syscall_entry(audit_arch(), regs->regs[3],
				    regs->regs[4], regs->regs[5],
				    regs->regs[6], regs->regs[7]);

	return ret ?: regs->regs[0];
}