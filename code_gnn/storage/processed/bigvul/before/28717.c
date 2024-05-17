static inline int apic_find_highest_irr(struct kvm_lapic *apic)
{
	int result;

	 
	if (!apic->irr_pending)
		return -1;

	kvm_x86_ops->sync_pir_to_irr(apic->vcpu);
	result = apic_search_irr(apic);
	ASSERT(result == -1 || result >= 16);

	return result;
}
