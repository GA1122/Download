static inline unsigned long *fetch_reg_addr(unsigned int reg, struct pt_regs *regs)
{
	struct reg_window32 *win;

	if(reg < 16)
		return &regs->u_regs[reg];
	win = (struct reg_window32 *) regs->u_regs[UREG_FP];
	return &win->locals[reg - 16];
}
