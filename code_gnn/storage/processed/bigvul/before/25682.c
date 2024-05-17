static int do_int_store(int reg_num, int size, unsigned long *dst_addr,
			struct pt_regs *regs)
{
	unsigned long zero[2] = { 0, 0 };
	unsigned long *src_val;

	if (reg_num)
		src_val = fetch_reg_addr(reg_num, regs);
	else {
		src_val = &zero[0];
		if (size == 8)
			zero[1] = fetch_reg(1, regs);
	}
	return __do_int_store(dst_addr, size, src_val);
}
