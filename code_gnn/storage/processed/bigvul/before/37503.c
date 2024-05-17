static int kvm_test_age_rmapp(struct kvm *kvm, unsigned long *rmapp,
			      struct kvm_memory_slot *slot, unsigned long data)
{
	u64 *sptep;
	struct rmap_iterator iter;
	int young = 0;

	 
	if (!shadow_accessed_mask)
		goto out;

	for (sptep = rmap_get_first(*rmapp, &iter); sptep;
	     sptep = rmap_get_next(&iter)) {
		BUG_ON(!is_shadow_present_pte(*sptep));

		if (*sptep & shadow_accessed_mask) {
			young = 1;
			break;
		}
	}
out:
	return young;
}
