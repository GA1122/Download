asmlinkage void do_illegal_slot_inst(unsigned long r4, unsigned long r5,
				unsigned long r6, unsigned long r7,
				struct pt_regs __regs)
{
	struct pt_regs *regs = RELOC_HIDE(&__regs, 0);
	unsigned long inst;
	struct task_struct *tsk = current;

	if (kprobe_handle_illslot(regs->pc) == 0)
		return;

#ifdef CONFIG_SH_FPU_EMU
	get_user(inst, (unsigned short *)regs->pc + 1);
	if (!do_fpu_inst(inst, regs)) {
		get_user(inst, (unsigned short *)regs->pc);
		if (!emulate_branch(inst, regs))
			return;
		 
	}
	 
#endif

	inst = lookup_exception_vector();

	local_irq_enable();
	force_sig(SIGILL, tsk);
	die_if_no_fixup("illegal slot instruction", regs, inst);
}