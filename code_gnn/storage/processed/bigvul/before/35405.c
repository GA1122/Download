show_stack_log_lvl(struct task_struct *task, struct pt_regs *regs,
		   unsigned long *sp, unsigned long bp, char *log_lvl)
{
	unsigned long *irq_stack_end;
	unsigned long *irq_stack;
	unsigned long *stack;
	int cpu;
	int i;

	preempt_disable();
	cpu = smp_processor_id();

	irq_stack_end	= (unsigned long *)(per_cpu(irq_stack_ptr, cpu));
	irq_stack	= (unsigned long *)(per_cpu(irq_stack_ptr, cpu) - IRQ_STACK_SIZE);

	 
	if (sp == NULL) {
		if (task)
			sp = (unsigned long *)task->thread.sp;
		else
			sp = (unsigned long *)&sp;
	}

	stack = sp;
	for (i = 0; i < kstack_depth_to_print; i++) {
		if (stack >= irq_stack && stack <= irq_stack_end) {
			if (stack == irq_stack_end) {
				stack = (unsigned long *) (irq_stack_end[-1]);
				pr_cont(" <EOI> ");
			}
		} else {
		if (((long) stack & (THREAD_SIZE-1)) == 0)
			break;
		}
		if (i && ((i % STACKSLOTS_PER_LINE) == 0))
			pr_cont("\n");
		pr_cont(" %016lx", *stack++);
		touch_nmi_watchdog();
	}
	preempt_enable();

	pr_cont("\n");
	show_trace_log_lvl(task, regs, sp, bp, log_lvl);
}