static int handle_vmon(struct kvm_vcpu *vcpu)
{
	int ret;
	gpa_t vmptr;
	struct page *page;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	const u64 VMXON_NEEDED_FEATURES = FEATURE_CONTROL_LOCKED
		| FEATURE_CONTROL_VMXON_ENABLED_OUTSIDE_SMX;

	 
	if (!kvm_read_cr4_bits(vcpu, X86_CR4_VMXE)) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	if (vmx->nested.vmxon) {
		nested_vmx_failValid(vcpu, VMXERR_VMXON_IN_VMX_ROOT_OPERATION);
		return kvm_skip_emulated_instruction(vcpu);
	}

	if ((vmx->msr_ia32_feature_control & VMXON_NEEDED_FEATURES)
			!= VMXON_NEEDED_FEATURES) {
		kvm_inject_gp(vcpu, 0);
		return 1;
	}

	if (nested_vmx_get_vmptr(vcpu, &vmptr))
		return 1;

	 
	if (!PAGE_ALIGNED(vmptr) || (vmptr >> cpuid_maxphyaddr(vcpu))) {
		nested_vmx_failInvalid(vcpu);
		return kvm_skip_emulated_instruction(vcpu);
	}

	page = kvm_vcpu_gpa_to_page(vcpu, vmptr);
	if (is_error_page(page)) {
		nested_vmx_failInvalid(vcpu);
		return kvm_skip_emulated_instruction(vcpu);
	}
	if (*(u32 *)kmap(page) != VMCS12_REVISION) {
		kunmap(page);
		kvm_release_page_clean(page);
		nested_vmx_failInvalid(vcpu);
		return kvm_skip_emulated_instruction(vcpu);
	}
	kunmap(page);
	kvm_release_page_clean(page);

	vmx->nested.vmxon_ptr = vmptr;
	ret = enter_vmx_operation(vcpu);
	if (ret)
		return ret;

	nested_vmx_succeed(vcpu);
	return kvm_skip_emulated_instruction(vcpu);
}
