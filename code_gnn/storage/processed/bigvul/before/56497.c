int sys_swapcontext(struct ucontext __user *old_ctx,
		    struct ucontext __user *new_ctx,
		    long ctx_size, long r6, long r7, long r8, struct pt_regs *regs)
{
	unsigned char tmp;
	sigset_t set;
	unsigned long new_msr = 0;
	int ctx_has_vsx_region = 0;

	if (new_ctx &&
	    get_user(new_msr, &new_ctx->uc_mcontext.gp_regs[PT_MSR]))
		return -EFAULT;
	 
	if (ctx_size < UCONTEXTSIZEWITHOUTVSX)
		return -EINVAL;
	 
	if ((ctx_size < sizeof(struct ucontext)) &&
	    (new_msr & MSR_VSX))
		return -EINVAL;
	 
	if (ctx_size >= sizeof(struct ucontext))
		ctx_has_vsx_region = 1;

	if (old_ctx != NULL) {
		if (!access_ok(VERIFY_WRITE, old_ctx, ctx_size)
		    || setup_sigcontext(&old_ctx->uc_mcontext, regs, 0, NULL, 0,
					ctx_has_vsx_region)
		    || __copy_to_user(&old_ctx->uc_sigmask,
				      &current->blocked, sizeof(sigset_t)))
			return -EFAULT;
	}
	if (new_ctx == NULL)
		return 0;
	if (!access_ok(VERIFY_READ, new_ctx, ctx_size)
	    || __get_user(tmp, (u8 __user *) new_ctx)
	    || __get_user(tmp, (u8 __user *) new_ctx + ctx_size - 1))
		return -EFAULT;

	 

	if (__copy_from_user(&set, &new_ctx->uc_sigmask, sizeof(set)))
		do_exit(SIGSEGV);
	set_current_blocked(&set);
	if (restore_sigcontext(regs, NULL, 0, &new_ctx->uc_mcontext))
		do_exit(SIGSEGV);

	 
	set_thread_flag(TIF_RESTOREALL);
	return 0;
}
