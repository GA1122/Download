void show_stack(struct task_struct *tsk, unsigned long *sp)
{
#ifdef CONFIG_KALLSYMS
	extern void sh64_unwind(struct pt_regs *regs);
	struct pt_regs *regs;

	regs = tsk ? tsk->thread.kregs : NULL;

	sh64_unwind(regs);
#else
	printk(KERN_ERR "Can't backtrace on sh64 without CONFIG_KALLSYMS\n");
#endif
}
