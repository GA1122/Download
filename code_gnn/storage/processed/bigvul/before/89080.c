static void __user *get_addr_ref_16(struct insn *insn, struct pt_regs *regs)
{
	unsigned long linear_addr = -1L, seg_base, seg_limit;
	int ret, regoff;
	short eff_addr;
	long tmp;

	insn_get_modrm(insn);
	insn_get_displacement(insn);

	if (insn->addr_bytes != 2)
		goto out;

	if (X86_MODRM_MOD(insn->modrm.value) == 3) {
		ret = get_eff_addr_reg(insn, regs, &regoff, &tmp);
		if (ret)
			goto out;

		eff_addr = tmp;
	} else {
		ret = get_eff_addr_modrm_16(insn, regs, &regoff, &eff_addr);
		if (ret)
			goto out;
	}

	ret = get_seg_base_limit(insn, regs, regoff, &seg_base, &seg_limit);
	if (ret)
		goto out;

	 
	if ((unsigned long)(eff_addr & 0xffff) > seg_limit)
		goto out;

	linear_addr = (unsigned long)(eff_addr & 0xffff) + seg_base;

	 
	if (v8086_mode(regs))
		linear_addr &= 0xfffff;

out:
	return (void __user *)linear_addr;
}