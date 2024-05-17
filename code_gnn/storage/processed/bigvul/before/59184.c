static void coerce_reg_to_size(struct bpf_reg_state *reg, int size)
{
	u64 mask;

	 
	reg->var_off = tnum_cast(reg->var_off, size);

	 
	mask = ((u64)1 << (size * 8)) - 1;
	if ((reg->umin_value & ~mask) == (reg->umax_value & ~mask)) {
		reg->umin_value &= mask;
		reg->umax_value &= mask;
	} else {
		reg->umin_value = 0;
		reg->umax_value = mask;
	}
	reg->smin_value = reg->umin_value;
	reg->smax_value = reg->umax_value;
}
