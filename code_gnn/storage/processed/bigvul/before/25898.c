static int getreg32(struct task_struct *child, unsigned regno, u32 *val)
{
	struct pt_regs *regs = task_pt_regs(child);

	switch (regno) {

	SEG32(ds);
	SEG32(es);
	SEG32(fs);
	SEG32(gs);

	R32(cs, cs);
	R32(ss, ss);
	R32(ebx, bx);
	R32(ecx, cx);
	R32(edx, dx);
	R32(edi, di);
	R32(esi, si);
	R32(ebp, bp);
	R32(eax, ax);
	R32(orig_eax, orig_ax);
	R32(eip, ip);
	R32(esp, sp);

	case offsetof(struct user32, regs.eflags):
		*val = get_flags(child);
		break;

	case offsetof(struct user32, u_debugreg[0]) ...
		offsetof(struct user32, u_debugreg[7]):
		regno -= offsetof(struct user32, u_debugreg[0]);
		*val = ptrace_get_debugreg(child, regno / 4);
		break;

	default:
		if (regno > sizeof(struct user32) || (regno & 3))
			return -EIO;

		 
		*val = 0;
		break;
	}
	return 0;
}