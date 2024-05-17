static long arch_ptrace_old(struct task_struct *child, long request,
			    unsigned long addr, unsigned long data)
{
	void __user *datavp = (void __user *) data;

	switch (request) {
	case PPC_PTRACE_GETREGS:	 
		return copy_regset_to_user(child, &user_ppc_native_view,
					   REGSET_GPR, 0, 32 * sizeof(long),
					   datavp);

	case PPC_PTRACE_SETREGS:	 
		return copy_regset_from_user(child, &user_ppc_native_view,
					     REGSET_GPR, 0, 32 * sizeof(long),
					     datavp);

	case PPC_PTRACE_GETFPREGS:	 
		return copy_regset_to_user(child, &user_ppc_native_view,
					   REGSET_FPR, 0, 32 * sizeof(double),
					   datavp);

	case PPC_PTRACE_SETFPREGS:	 
		return copy_regset_from_user(child, &user_ppc_native_view,
					     REGSET_FPR, 0, 32 * sizeof(double),
					     datavp);
	}

	return -EPERM;
}
