asmlinkage void do_reserved_inst(unsigned long r4, unsigned long r5,
				unsigned long r6, unsigned long r7,
				struct pt_regs __regs)
{
	struct pt_regs *regs = RELOC_HIDE(&__regs, 0);
	unsigned long error_code;
	struct task_struct *tsk = current;

#ifdef CONFIG_SH_FPU_EMU
	unsigned short inst = 0;
	int err;

	get_user(inst, (unsigned short*)regs->pc);

	err = do_fpu_inst(inst, regs);
	if (!err) {
		regs->pc += instruction_size(inst);
		return;
	}
	 
#endif

#ifdef CONFIG_SH_DSP
	 
	if (is_dsp_inst(regs)) {
		 
		regs->sr |= SR_DSP;
		 
		tsk->thread.dsp_status.status |= SR_DSP;
		return;
	}
#endif

	error_code = lookup_exception_vector();

	local_irq_enable();
	force_sig(SIGILL, tsk);
	die_if_no_fixup("reserved instruction", regs, error_code);
}