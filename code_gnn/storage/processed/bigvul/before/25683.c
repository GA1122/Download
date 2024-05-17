static inline unsigned long fetch_reg(unsigned int reg, struct pt_regs *regs)
{
	struct reg_window32 *win;

	if(reg < 16)
		return (!reg ? 0 : regs->u_regs[reg]);

	 
	win = (struct reg_window32 *) regs->u_regs[UREG_FP];
	return win->locals[reg - 16];  
}
