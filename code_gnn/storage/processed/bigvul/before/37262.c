static void vmx_hwapic_irr_update(struct kvm_vcpu *vcpu, int max_irr)
{
	if (max_irr == -1)
		return;

	 
	if (is_guest_mode(vcpu) && nested_exit_on_intr(vcpu))
		return;

	if (!is_guest_mode(vcpu)) {
		vmx_set_rvi(max_irr);
		return;
	}

	 
	if (!kvm_event_needs_reinjection(vcpu) &&
	    vmx_interrupt_allowed(vcpu)) {
		kvm_queue_interrupt(vcpu, max_irr, false);
		vmx_inject_irq(vcpu);
	}
}