static u8 mtrr_disabled_type(struct kvm_vcpu *vcpu)
{
	 
	if (guest_cpuid_has_mtrr(vcpu))
		return MTRR_TYPE_UNCACHABLE;
	else
		return MTRR_TYPE_WRBACK;
}
