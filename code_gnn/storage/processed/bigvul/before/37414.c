static bool fast_page_fault(struct kvm_vcpu *vcpu, gva_t gva, int level,
			    u32 error_code)
{
	struct kvm_shadow_walk_iterator iterator;
	bool ret = false;
	u64 spte = 0ull;

	if (!page_fault_can_be_fast(vcpu, error_code))
		return false;

	walk_shadow_page_lockless_begin(vcpu);
	for_each_shadow_entry_lockless(vcpu, gva, iterator, spte)
		if (!is_shadow_present_pte(spte) || iterator.level < level)
			break;

	 
	if (!is_rmap_spte(spte)) {
		ret = true;
		goto exit;
	}

	if (!is_last_spte(spte, level))
		goto exit;

	 
	 if (is_writable_pte(spte)) {
		ret = true;
		goto exit;
	}

	 
	if (!spte_is_locklessly_modifiable(spte))
		goto exit;

	 
	ret = fast_pf_fix_direct_spte(vcpu, iterator.sptep, spte);
exit:
	trace_fast_page_fault(vcpu, gva, error_code, iterator.sptep,
			      spte, ret);
	walk_shadow_page_lockless_end(vcpu);

	return ret;
}
