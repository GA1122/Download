void show_stack(struct task_struct *task, unsigned long *sp)
{
	struct pt_regs regs;
	if (sp) {
		regs.regs[29] = (unsigned long)sp;
		regs.regs[31] = 0;
		regs.cp0_epc = 0;
	} else {
		if (task && task != current) {
			regs.regs[29] = task->thread.reg29;
			regs.regs[31] = 0;
			regs.cp0_epc = task->thread.reg31;
#ifdef CONFIG_KGDB_KDB
		} else if (atomic_read(&kgdb_active) != -1 &&
			   kdb_current_regs) {
			memcpy(&regs, kdb_current_regs, sizeof(regs));
#endif  
		} else {
			prepare_frametrace(&regs);
		}
	}
	show_stacktrace(task, &regs);
}