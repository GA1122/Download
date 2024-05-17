struct bad_iret_stack *fixup_bad_iret(struct bad_iret_stack *s)
{
	 
	struct bad_iret_stack *new_stack =
		(struct bad_iret_stack *)this_cpu_read(cpu_tss_rw.x86_tss.sp0) - 1;

	 
	memmove(&new_stack->regs.ip, (void *)s->regs.sp, 5*8);

	 
	memmove(new_stack, s, offsetof(struct bad_iret_stack, regs.ip));

	BUG_ON(!user_mode(&new_stack->regs));
	return new_stack;
}
