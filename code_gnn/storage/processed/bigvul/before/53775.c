static void set_var_mtrr_msr(struct kvm_vcpu *vcpu, u32 msr, u64 data)
{
	struct kvm_mtrr *mtrr_state = &vcpu->arch.mtrr_state;
	struct kvm_mtrr_range *tmp, *cur;
	int index, is_mtrr_mask;

	index = (msr - 0x200) / 2;
	is_mtrr_mask = msr - 0x200 - 2 * index;
	cur = &mtrr_state->var_ranges[index];

	 
	if (var_mtrr_range_is_valid(cur))
		list_del(&mtrr_state->var_ranges[index].node);

	 
	if (!is_mtrr_mask)
		cur->base = data;
	else
		cur->mask = data | (-1LL << cpuid_maxphyaddr(vcpu));

	 
	if (var_mtrr_range_is_valid(cur)) {
		list_for_each_entry(tmp, &mtrr_state->head, node)
			if (cur->base >= tmp->base)
				break;
		list_add_tail(&cur->node, &tmp->node);
	}
}
