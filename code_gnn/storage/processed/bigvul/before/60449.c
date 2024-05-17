int copy_fpstate_to_sigframe(void __user *buf, void __user *buf_fx, int size)
{
	struct fpu *fpu = &current->thread.fpu;
	struct xregs_state *xsave = &fpu->state.xsave;
	struct task_struct *tsk = current;
	int ia32_fxstate = (buf != buf_fx);

	ia32_fxstate &= (IS_ENABLED(CONFIG_X86_32) ||
			 IS_ENABLED(CONFIG_IA32_EMULATION));

	if (!access_ok(VERIFY_WRITE, buf, size))
		return -EACCES;

	if (!static_cpu_has(X86_FEATURE_FPU))
		return fpregs_soft_get(current, NULL, 0,
			sizeof(struct user_i387_ia32_struct), NULL,
			(struct _fpstate_32 __user *) buf) ? -1 : 1;

	if (fpu->fpstate_active || using_compacted_format()) {
		 
		if (copy_fpregs_to_sigframe(buf_fx))
			return -1;
		 
		if (ia32_fxstate)
			copy_fxregs_to_kernel(fpu);
	} else {
		 
		if (boot_cpu_has(X86_FEATURE_XSAVES)) {
			WARN_ONCE(1, "x86/fpu: saving compacted-format xsave area to a signal frame!\n");
			return -1;
		}

		fpstate_sanitize_xstate(fpu);
		if (__copy_to_user(buf_fx, xsave, fpu_user_xstate_size))
			return -1;
	}

	 
	if ((ia32_fxstate || !use_fxsr()) && save_fsave_header(tsk, buf))
		return -1;

	if (use_fxsr() && save_xstate_epilog(buf_fx, ia32_fxstate))
		return -1;

	return 0;
}
