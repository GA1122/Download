trace_get_syscall_nr(struct task_struct *task, struct pt_regs *regs)
{
	if (unlikely(arch_trace_is_compat_syscall(regs)))
		return -1;

	return syscall_get_nr(task, regs);
}
