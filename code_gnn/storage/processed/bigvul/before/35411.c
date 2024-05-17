 dotraplinkage void do_double_fault(struct pt_regs *regs, long error_code)
 {
 	static const char str[] = "double fault";
	struct task_struct *tsk = current;

#ifdef CONFIG_X86_ESPFIX64
	extern unsigned char native_irq_return_iret[];

	 
	if (((long)regs->sp >> PGDIR_SHIFT) == ESPFIX_PGD_ENTRY &&
		regs->cs == __KERNEL_CS &&
		regs->ip == (unsigned long)native_irq_return_iret)
	{
		struct pt_regs *normal_regs = task_pt_regs(current);

		 
		memmove(&normal_regs->ip, (void *)regs->sp, 5*8);
		normal_regs->orig_ax = 0;   
		regs->ip = (unsigned long)general_protection;
		regs->sp = (unsigned long)&normal_regs->orig_ax;
		return;
	}
#endif

	exception_enter();
	 
	notify_die(DIE_TRAP, str, regs, error_code, X86_TRAP_DF, SIGSEGV);

	tsk->thread.error_code = error_code;
	tsk->thread.trap_nr = X86_TRAP_DF;

#ifdef CONFIG_DOUBLEFAULT
	df_debug(regs, error_code);
#endif
	 
	for (;;)
		die(str, regs, error_code);
}
