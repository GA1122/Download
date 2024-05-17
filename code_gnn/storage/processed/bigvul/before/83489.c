dotraplinkage void do_double_fault(struct pt_regs *regs, long error_code)
{
	static const char str[] = "double fault";
	struct task_struct *tsk = current;
#ifdef CONFIG_VMAP_STACK
	unsigned long cr2;
#endif

#ifdef CONFIG_X86_ESPFIX64
	extern unsigned char native_irq_return_iret[];

	 
	if (((long)regs->sp >> P4D_SHIFT) == ESPFIX_PGD_ENTRY &&
		regs->cs == __KERNEL_CS &&
		regs->ip == (unsigned long)native_irq_return_iret)
	{
		struct pt_regs *gpregs = (struct pt_regs *)this_cpu_read(cpu_tss_rw.x86_tss.sp0) - 1;

		 
		memmove(&gpregs->ip, (void *)regs->sp, 5*8);
		gpregs->orig_ax = 0;   

		 
		regs->ip = (unsigned long)general_protection;
		regs->sp = (unsigned long)&gpregs->orig_ax;

		return;
	}
#endif

	ist_enter(regs);
	notify_die(DIE_TRAP, str, regs, error_code, X86_TRAP_DF, SIGSEGV);

	tsk->thread.error_code = error_code;
	tsk->thread.trap_nr = X86_TRAP_DF;

#ifdef CONFIG_VMAP_STACK
	 
	cr2 = read_cr2();
	if ((unsigned long)task_stack_page(tsk) - 1 - cr2 < PAGE_SIZE)
		handle_stack_overflow("kernel stack overflow (double-fault)", regs, cr2);
#endif

#ifdef CONFIG_DOUBLEFAULT
	df_debug(regs, error_code);
#endif
	 
	for (;;)
		die(str, regs, error_code);
}
