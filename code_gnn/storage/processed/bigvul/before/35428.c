asmlinkage __visible struct pt_regs *sync_regs(struct pt_regs *eregs)
{
	struct pt_regs *regs = eregs;
	 
	if (eregs == (struct pt_regs *)eregs->sp)
		;
	 
	else if (user_mode(eregs))
		regs = task_pt_regs(current);
	 
	else if (eregs->flags & X86_EFLAGS_IF)
		regs = (struct pt_regs *)(eregs->sp -= sizeof(struct pt_regs));
	if (eregs != regs)
		*regs = *eregs;
	return regs;
}
