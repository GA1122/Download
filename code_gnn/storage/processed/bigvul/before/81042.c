static void vmx_hwapic_irr_update(struct kvm_vcpu *vcpu, int max_irr)
{
	 
	if (!is_guest_mode(vcpu))
		vmx_set_rvi(max_irr);
}
