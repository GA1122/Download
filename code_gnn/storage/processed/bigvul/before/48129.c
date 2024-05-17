vmx_restore_control_msr(struct vcpu_vmx *vmx, u32 msr_index, u64 data)
{
	u64 supported;
	u32 *lowp, *highp;

	switch (msr_index) {
	case MSR_IA32_VMX_TRUE_PINBASED_CTLS:
		lowp = &vmx->nested.nested_vmx_pinbased_ctls_low;
		highp = &vmx->nested.nested_vmx_pinbased_ctls_high;
		break;
	case MSR_IA32_VMX_TRUE_PROCBASED_CTLS:
		lowp = &vmx->nested.nested_vmx_procbased_ctls_low;
		highp = &vmx->nested.nested_vmx_procbased_ctls_high;
		break;
	case MSR_IA32_VMX_TRUE_EXIT_CTLS:
		lowp = &vmx->nested.nested_vmx_exit_ctls_low;
		highp = &vmx->nested.nested_vmx_exit_ctls_high;
		break;
	case MSR_IA32_VMX_TRUE_ENTRY_CTLS:
		lowp = &vmx->nested.nested_vmx_entry_ctls_low;
		highp = &vmx->nested.nested_vmx_entry_ctls_high;
		break;
	case MSR_IA32_VMX_PROCBASED_CTLS2:
		lowp = &vmx->nested.nested_vmx_secondary_ctls_low;
		highp = &vmx->nested.nested_vmx_secondary_ctls_high;
		break;
	default:
		BUG();
	}

	supported = vmx_control_msr(*lowp, *highp);

	 
	if (!is_bitwise_subset(data, supported, GENMASK_ULL(31, 0)))
		return -EINVAL;

	 
	if (!is_bitwise_subset(supported, data, GENMASK_ULL(63, 32)))
		return -EINVAL;

	*lowp = data;
	*highp = data >> 32;
	return 0;
}