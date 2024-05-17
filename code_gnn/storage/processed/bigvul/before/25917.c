long syscall_trace_enter(struct pt_regs *regs)
{
	long ret = 0;

	 
	if (test_thread_flag(TIF_SINGLESTEP))
		regs->flags |= X86_EFLAGS_TF;

	 
	secure_computing(regs->orig_ax);

	if (unlikely(test_thread_flag(TIF_SYSCALL_EMU)))
		ret = -1L;

	if ((ret || test_thread_flag(TIF_SYSCALL_TRACE)) &&
	    tracehook_report_syscall_entry(regs))
		ret = -1L;

	if (unlikely(test_thread_flag(TIF_SYSCALL_TRACEPOINT)))
		trace_sys_enter(regs, regs->orig_ax);

	if (unlikely(current->audit_context)) {
		if (IS_IA32)
			audit_syscall_entry(AUDIT_ARCH_I386,
					    regs->orig_ax,
					    regs->bx, regs->cx,
					    regs->dx, regs->si);
#ifdef CONFIG_X86_64
		else
			audit_syscall_entry(AUDIT_ARCH_X86_64,
					    regs->orig_ax,
					    regs->di, regs->si,
					    regs->dx, regs->r10);
#endif
	}

	return ret ?: regs->orig_ax;
}