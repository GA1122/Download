u8 kvm_mtrr_get_guest_memory_type(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	struct kvm_mtrr *mtrr_state = &vcpu->arch.mtrr_state;
	struct mtrr_iter iter;
	u64 start, end;
	int type = -1;
	const int wt_wb_mask = (1 << MTRR_TYPE_WRBACK)
			       | (1 << MTRR_TYPE_WRTHROUGH);

	start = gfn_to_gpa(gfn);
	end = start + PAGE_SIZE;

	mtrr_for_each_mem_type(&iter, mtrr_state, start, end) {
		int curr_type = iter.mem_type;

		 

		if (type == -1) {
			type = curr_type;
			continue;
		}

		 
		if (type == curr_type)
			continue;

		 
		if (curr_type == MTRR_TYPE_UNCACHABLE)
			return MTRR_TYPE_UNCACHABLE;

		 
		if (((1 << type) & wt_wb_mask) &&
		      ((1 << curr_type) & wt_wb_mask)) {
			type = MTRR_TYPE_WRTHROUGH;
			continue;
		}

		 

		 
		return MTRR_TYPE_WRBACK;
	}

	if (iter.mtrr_disabled)
		return mtrr_disabled_type(vcpu);

	 
	if (type == -1)
		return mtrr_default_type(mtrr_state);

	 
	WARN_ON(iter.partial_map);

	return type;
}
