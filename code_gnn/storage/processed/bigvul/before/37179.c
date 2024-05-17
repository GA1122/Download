static bool tr_valid(struct kvm_vcpu *vcpu)
{
	struct kvm_segment tr;

	vmx_get_segment(vcpu, &tr, VCPU_SREG_TR);

	if (tr.unusable)
		return false;
	if (tr.selector & SELECTOR_TI_MASK)	 
		return false;
	if (tr.type != 3 && tr.type != 11)  
		return false;
	if (!tr.present)
		return false;

	return true;
}
