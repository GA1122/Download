long sys_swapcontext(struct ucontext __user *old_ctx,
		     struct ucontext __user *new_ctx,
		     int ctx_size, int r6, int r7, int r8, struct pt_regs *regs)
{
	unsigned char tmp;
	int ctx_has_vsx_region = 0;

#ifdef CONFIG_PPC64
	unsigned long new_msr = 0;

	if (new_ctx) {
		struct mcontext __user *mcp;
		u32 cmcp;

		 
		if (__get_user(cmcp, &new_ctx->uc_regs))
			return -EFAULT;
		mcp = (struct mcontext __user *)(u64)cmcp;
		if (__get_user(new_msr, &mcp->mc_gregs[PT_MSR]))
			return -EFAULT;
	}
	 
	if (ctx_size < UCONTEXTSIZEWITHOUTVSX)
		return -EINVAL;
	 
	if ((ctx_size < sizeof(struct ucontext)) &&
	    (new_msr & MSR_VSX))
		return -EINVAL;
	 
	if (ctx_size >= sizeof(struct ucontext))
		ctx_has_vsx_region = 1;
#else
	 
	if (ctx_size < sizeof(struct ucontext))
		return -EINVAL;
#endif
	if (old_ctx != NULL) {
		struct mcontext __user *mctx;

		 
		mctx = (struct mcontext __user *)
			((unsigned long) &old_ctx->uc_mcontext & ~0xfUL);
		if (!access_ok(VERIFY_WRITE, old_ctx, ctx_size)
		    || save_user_regs(regs, mctx, NULL, 0, ctx_has_vsx_region)
		    || put_sigset_t(&old_ctx->uc_sigmask, &current->blocked)
		    || __put_user(to_user_ptr(mctx), &old_ctx->uc_regs))
			return -EFAULT;
	}
	if (new_ctx == NULL)
		return 0;
	if (!access_ok(VERIFY_READ, new_ctx, ctx_size)
	    || __get_user(tmp, (u8 __user *) new_ctx)
	    || __get_user(tmp, (u8 __user *) new_ctx + ctx_size - 1))
		return -EFAULT;

	 
	if (do_setcontext(new_ctx, regs, 0))
		do_exit(SIGSEGV);

	set_thread_flag(TIF_RESTOREALL);
	return 0;
}
