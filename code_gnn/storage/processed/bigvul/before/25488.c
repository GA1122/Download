unsigned long ptrace_get_reg(struct task_struct *task, int regno)
{
	if (task->thread.regs == NULL)
		return -EIO;

	if (regno == PT_MSR)
		return get_user_msr(task);

	if (regno < (sizeof(struct pt_regs) / sizeof(unsigned long)))
		return ((unsigned long *)task->thread.regs)[regno];

	return -EIO;
}
