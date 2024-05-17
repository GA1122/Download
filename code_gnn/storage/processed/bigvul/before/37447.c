static int kvm_age_rmapp(struct kvm *kvm, unsigned long *rmapp,
			 struct kvm_memory_slot *slot, unsigned long data)
{
	u64 *sptep;
	struct rmap_iterator uninitialized_var(iter);
	int young = 0;

	 
	if (!shadow_accessed_mask) {
		young = kvm_unmap_rmapp(kvm, rmapp, slot, data);
		goto out;
	}

	for (sptep = rmap_get_first(*rmapp, &iter); sptep;
	     sptep = rmap_get_next(&iter)) {
		BUG_ON(!is_shadow_present_pte(*sptep));

		if (*sptep & shadow_accessed_mask) {
			young = 1;
			clear_bit((ffs(shadow_accessed_mask) - 1),
				 (unsigned long *)sptep);
		}
	}
out:
	 
	trace_kvm_age_page(data, slot, young);
	return young;
}
