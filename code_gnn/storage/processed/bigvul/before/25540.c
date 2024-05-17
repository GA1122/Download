static inline int put_stack_long(struct task_struct *task, int offset,
				 unsigned long data)
{
	unsigned char *stack;

	stack = (unsigned char *)task_pt_regs(task);
	stack += offset;
	*(unsigned long *) stack = data;
 	return 0;
 }