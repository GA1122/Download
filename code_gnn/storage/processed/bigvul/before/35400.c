void dump_trace(struct task_struct *task, struct pt_regs *regs,
		unsigned long *stack, unsigned long bp,
		const struct stacktrace_ops *ops, void *data)
{
	const unsigned cpu = get_cpu();
	struct thread_info *tinfo;
	unsigned long *irq_stack = (unsigned long *)per_cpu(irq_stack_ptr, cpu);
	unsigned long dummy;
	unsigned used = 0;
	int graph = 0;
	int done = 0;

	if (!task)
		task = current;

	if (!stack) {
		if (regs)
			stack = (unsigned long *)regs->sp;
		else if (task != current)
			stack = (unsigned long *)task->thread.sp;
		else
			stack = &dummy;
	}

	if (!bp)
		bp = stack_frame(task, regs);
	 
	tinfo = task_thread_info(task);
	while (!done) {
		unsigned long *stack_end;
		enum stack_type stype;
		char *id;

		stype = analyze_stack(cpu, task, stack, &stack_end,
				      irq_stack, &used, &id);

		 
		done = 1;

		switch (stype) {

		 
		case STACK_IS_NORMAL:
			break;

		case STACK_IS_EXCEPTION:

			if (ops->stack(data, id) < 0)
				break;

			bp = ops->walk_stack(tinfo, stack, bp, ops,
					     data, stack_end, &graph);
			ops->stack(data, "<EOE>");
			 
			stack = (unsigned long *) stack_end[-2];
			done = 0;
			break;

		case STACK_IS_IRQ:

			if (ops->stack(data, "IRQ") < 0)
				break;
			bp = ops->walk_stack(tinfo, stack, bp,
				     ops, data, stack_end, &graph);
			 
			stack = (unsigned long *) (stack_end[-1]);
			irq_stack = NULL;
			ops->stack(data, "EOI");
			done = 0;
			break;

		case STACK_IS_UNKNOWN:
			ops->stack(data, "UNK");
			break;
		}
	}

	 
	bp = ops->walk_stack(tinfo, stack, bp, ops, data, NULL, &graph);
	put_cpu();
}
