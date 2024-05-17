static int do_setcontext_tm(struct ucontext __user *ucp,
			    struct ucontext __user *tm_ucp,
			    struct pt_regs *regs)
{
	sigset_t set;
	struct mcontext __user *mcp;
	struct mcontext __user *tm_mcp;
	u32 cmcp;
	u32 tm_cmcp;

	if (get_sigset_t(&set, &ucp->uc_sigmask))
		return -EFAULT;

	if (__get_user(cmcp, &ucp->uc_regs) ||
	    __get_user(tm_cmcp, &tm_ucp->uc_regs))
		return -EFAULT;
	mcp = (struct mcontext __user *)(u64)cmcp;
	tm_mcp = (struct mcontext __user *)(u64)tm_cmcp;
	 

	set_current_blocked(&set);
	if (restore_tm_user_regs(regs, mcp, tm_mcp))
		return -EFAULT;

	return 0;
}