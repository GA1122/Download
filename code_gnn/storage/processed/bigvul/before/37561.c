static bool page_fault_can_be_fast(struct kvm_vcpu *vcpu, u32 error_code)
{
	 
	if (unlikely(error_code & PFERR_RSVD_MASK))
		return false;

	 
	if (!(error_code & PFERR_PRESENT_MASK) ||
	      !(error_code & PFERR_WRITE_MASK))
		return false;

	return true;
}
