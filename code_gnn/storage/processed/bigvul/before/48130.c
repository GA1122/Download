static int vmx_restore_fixed0_msr(struct vcpu_vmx *vmx, u32 msr_index, u64 data)
{
	u64 *msr;

	switch (msr_index) {
	case MSR_IA32_VMX_CR0_FIXED0:
		msr = &vmx->nested.nested_vmx_cr0_fixed0;
		break;
	case MSR_IA32_VMX_CR4_FIXED0:
		msr = &vmx->nested.nested_vmx_cr4_fixed0;
		break;
	default:
		BUG();
	}

	 
	if (!is_bitwise_subset(data, *msr, -1ULL))
		return -EINVAL;

	*msr = data;
	return 0;
}
