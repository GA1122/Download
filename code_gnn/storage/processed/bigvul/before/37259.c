static int vmx_get_vmx_msr(struct kvm_vcpu *vcpu, u32 msr_index, u64 *pdata)
{
	switch (msr_index) {
	case MSR_IA32_VMX_BASIC:
		 
		*pdata = VMCS12_REVISION | VMX_BASIC_TRUE_CTLS |
			   ((u64)VMCS12_SIZE << VMX_BASIC_VMCS_SIZE_SHIFT) |
			   (VMX_BASIC_MEM_TYPE_WB << VMX_BASIC_MEM_TYPE_SHIFT);
		break;
	case MSR_IA32_VMX_TRUE_PINBASED_CTLS:
	case MSR_IA32_VMX_PINBASED_CTLS:
		*pdata = vmx_control_msr(nested_vmx_pinbased_ctls_low,
					nested_vmx_pinbased_ctls_high);
		break;
	case MSR_IA32_VMX_TRUE_PROCBASED_CTLS:
		*pdata = vmx_control_msr(nested_vmx_true_procbased_ctls_low,
					nested_vmx_procbased_ctls_high);
		break;
	case MSR_IA32_VMX_PROCBASED_CTLS:
		*pdata = vmx_control_msr(nested_vmx_procbased_ctls_low,
					nested_vmx_procbased_ctls_high);
		break;
	case MSR_IA32_VMX_TRUE_EXIT_CTLS:
		*pdata = vmx_control_msr(nested_vmx_true_exit_ctls_low,
					nested_vmx_exit_ctls_high);
		break;
	case MSR_IA32_VMX_EXIT_CTLS:
		*pdata = vmx_control_msr(nested_vmx_exit_ctls_low,
					nested_vmx_exit_ctls_high);
		break;
	case MSR_IA32_VMX_TRUE_ENTRY_CTLS:
		*pdata = vmx_control_msr(nested_vmx_true_entry_ctls_low,
					nested_vmx_entry_ctls_high);
		break;
	case MSR_IA32_VMX_ENTRY_CTLS:
		*pdata = vmx_control_msr(nested_vmx_entry_ctls_low,
					nested_vmx_entry_ctls_high);
		break;
	case MSR_IA32_VMX_MISC:
		*pdata = vmx_control_msr(nested_vmx_misc_low,
					 nested_vmx_misc_high);
		break;
	 
#define VMXON_CR0_ALWAYSON	(X86_CR0_PE | X86_CR0_PG | X86_CR0_NE)
#define VMXON_CR4_ALWAYSON	X86_CR4_VMXE
	case MSR_IA32_VMX_CR0_FIXED0:
		*pdata = VMXON_CR0_ALWAYSON;
		break;
	case MSR_IA32_VMX_CR0_FIXED1:
		*pdata = -1ULL;
		break;
	case MSR_IA32_VMX_CR4_FIXED0:
		*pdata = VMXON_CR4_ALWAYSON;
		break;
	case MSR_IA32_VMX_CR4_FIXED1:
		*pdata = -1ULL;
		break;
	case MSR_IA32_VMX_VMCS_ENUM:
		*pdata = 0x2e;  
		break;
	case MSR_IA32_VMX_PROCBASED_CTLS2:
		*pdata = vmx_control_msr(nested_vmx_secondary_ctls_low,
					nested_vmx_secondary_ctls_high);
		break;
	case MSR_IA32_VMX_EPT_VPID_CAP:
		 
		*pdata = nested_vmx_ept_caps;
		break;
	default:
		return 1;
	}

	return 0;
}
