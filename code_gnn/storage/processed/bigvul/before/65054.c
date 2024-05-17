static bool compare_ptrs_to_packet(struct bpf_reg_state *old,
				   struct bpf_reg_state *cur)
{
	if (old->id != cur->id)
		return false;

	 
	if (old->off == cur->off && old->range < cur->range)
		return true;

	 
	if (old->off <= cur->off &&
	    old->off >= old->range && cur->off >= cur->range)
		return true;

	return false;
}
