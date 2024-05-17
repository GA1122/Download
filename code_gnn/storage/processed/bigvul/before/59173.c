static bool stacksafe(struct bpf_verifier_state *old,
		      struct bpf_verifier_state *cur,
		      struct idpair *idmap)
{
	int i, spi;

	 
	if (old->allocated_stack > cur->allocated_stack)
		return false;

	 
	for (i = 0; i < old->allocated_stack; i++) {
		spi = i / BPF_REG_SIZE;

		if (old->stack[spi].slot_type[i % BPF_REG_SIZE] == STACK_INVALID)
			continue;
		if (old->stack[spi].slot_type[i % BPF_REG_SIZE] !=
		    cur->stack[spi].slot_type[i % BPF_REG_SIZE])
			 
			return false;
		if (i % BPF_REG_SIZE)
			continue;
		if (old->stack[spi].slot_type[0] != STACK_SPILL)
			continue;
		if (!regsafe(&old->stack[spi].spilled_ptr,
			     &cur->stack[spi].spilled_ptr,
			     idmap))
			 
			return false;
	}
	return true;
}
