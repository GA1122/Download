do_device_not_available(struct pt_regs *regs, long error_code)
{
	enum ctx_state prev_state;

	prev_state = exception_enter();
	BUG_ON(use_eager_fpu());

#ifdef CONFIG_MATH_EMULATION
	if (read_cr0() & X86_CR0_EM) {
		struct math_emu_info info = { };

		conditional_sti(regs);

		info.regs = regs;
		math_emulate(&info);
		exception_exit(prev_state);
		return;
	}
#endif
	math_state_restore();  
#ifdef CONFIG_X86_32
	conditional_sti(regs);
#endif
	exception_exit(prev_state);
}