static void apic_sync_pv_eoi_to_guest(struct kvm_vcpu *vcpu,
					struct kvm_lapic *apic)
{
	if (!pv_eoi_enabled(vcpu) ||
	     
	    apic->irr_pending ||
	     
	    apic->highest_isr_cache == -1 ||
	     
	    kvm_ioapic_handles_vector(vcpu->kvm, apic->highest_isr_cache)) {
		 
		return;
	}

	pv_eoi_set_pending(apic->vcpu);
}
