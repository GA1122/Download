static void __mark_reg_unknown(struct bpf_reg_state *reg)
{
	 
	memset(reg, 0, offsetof(struct bpf_reg_state, var_off));
	reg->type = SCALAR_VALUE;
	reg->var_off = tnum_unknown;
	reg->frameno = 0;
	__mark_reg_unbounded(reg);
}
