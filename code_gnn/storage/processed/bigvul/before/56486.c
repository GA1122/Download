int sys_debug_setcontext(struct ucontext __user *ctx,
			 int ndbg, struct sig_dbg_op __user *dbg,
			 int r6, int r7, int r8,
			 struct pt_regs *regs)
{
	struct sig_dbg_op op;
	int i;
	unsigned char tmp;
	unsigned long new_msr = regs->msr;
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
	unsigned long new_dbcr0 = current->thread.debug.dbcr0;
#endif

	for (i=0; i<ndbg; i++) {
		if (copy_from_user(&op, dbg + i, sizeof(op)))
			return -EFAULT;
		switch (op.dbg_type) {
		case SIG_DBG_SINGLE_STEPPING:
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
			if (op.dbg_value) {
				new_msr |= MSR_DE;
				new_dbcr0 |= (DBCR0_IDM | DBCR0_IC);
			} else {
				new_dbcr0 &= ~DBCR0_IC;
				if (!DBCR_ACTIVE_EVENTS(new_dbcr0,
						current->thread.debug.dbcr1)) {
					new_msr &= ~MSR_DE;
					new_dbcr0 &= ~DBCR0_IDM;
				}
			}
#else
			if (op.dbg_value)
				new_msr |= MSR_SE;
			else
				new_msr &= ~MSR_SE;
#endif
			break;
		case SIG_DBG_BRANCH_TRACING:
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
			return -EINVAL;
#else
			if (op.dbg_value)
				new_msr |= MSR_BE;
			else
				new_msr &= ~MSR_BE;
#endif
			break;

		default:
			return -EINVAL;
		}
	}

	 
	regs->msr = new_msr;
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
	current->thread.debug.dbcr0 = new_dbcr0;
#endif

	if (!access_ok(VERIFY_READ, ctx, sizeof(*ctx))
	    || __get_user(tmp, (u8 __user *) ctx)
	    || __get_user(tmp, (u8 __user *) (ctx + 1) - 1))
		return -EFAULT;

	 
	if (do_setcontext(ctx, regs, 1)) {
		if (show_unhandled_signals)
			printk_ratelimited(KERN_INFO "%s[%d]: bad frame in "
					   "sys_debug_setcontext: %p nip %08lx "
					   "lr %08lx\n",
					   current->comm, current->pid,
					   ctx, regs->nip, regs->link);

		force_sig(SIGSEGV, current);
		goto out;
	}

	 
	restore_altstack(&ctx->uc_stack);

	set_thread_flag(TIF_RESTOREALL);
 out:
	return 0;
}
