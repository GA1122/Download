long compat_arch_ptrace(struct task_struct *child, compat_long_t request,
			compat_ulong_t caddr, compat_ulong_t cdata)
{
	unsigned long addr = caddr;
	unsigned long data = cdata;
	void __user *datap = compat_ptr(data);
	int ret;
	__u32 val;

	switch (request) {
	case PTRACE_PEEKUSR:
		ret = getreg32(child, addr, &val);
		if (ret == 0)
			ret = put_user(val, (__u32 __user *)datap);
		break;

	case PTRACE_POKEUSR:
		ret = putreg32(child, addr, data);
		break;

	case PTRACE_GETREGS:	 
		return copy_regset_to_user(child, &user_x86_32_view,
					   REGSET_GENERAL,
					   0, sizeof(struct user_regs_struct32),
					   datap);

	case PTRACE_SETREGS:	 
		return copy_regset_from_user(child, &user_x86_32_view,
					     REGSET_GENERAL, 0,
					     sizeof(struct user_regs_struct32),
					     datap);

	case PTRACE_GETFPREGS:	 
		return copy_regset_to_user(child, &user_x86_32_view,
					   REGSET_FP, 0,
					   sizeof(struct user_i387_ia32_struct),
					   datap);

	case PTRACE_SETFPREGS:	 
		return copy_regset_from_user(
			child, &user_x86_32_view, REGSET_FP,
			0, sizeof(struct user_i387_ia32_struct), datap);

	case PTRACE_GETFPXREGS:	 
		return copy_regset_to_user(child, &user_x86_32_view,
					   REGSET_XFP, 0,
					   sizeof(struct user32_fxsr_struct),
					   datap);

	case PTRACE_SETFPXREGS:	 
		return copy_regset_from_user(child, &user_x86_32_view,
					     REGSET_XFP, 0,
					     sizeof(struct user32_fxsr_struct),
					     datap);

	case PTRACE_GET_THREAD_AREA:
	case PTRACE_SET_THREAD_AREA:
		return arch_ptrace(child, request, addr, data);

	default:
		return compat_ptrace_request(child, request, addr, data);
	}

	return ret;
}