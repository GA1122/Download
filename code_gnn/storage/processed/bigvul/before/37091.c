static int handle_vmclear(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	gpa_t vmptr;
	struct vmcs12 *vmcs12;
	struct page *page;

	if (!nested_vmx_check_permission(vcpu))
		return 1;

	if (nested_vmx_check_vmptr(vcpu, EXIT_REASON_VMCLEAR, &vmptr))
		return 1;

	if (vmptr == vmx->nested.current_vmptr)
		nested_release_vmcs12(vmx);

	page = nested_get_page(vcpu, vmptr);
	if (page == NULL) {
		 
		kvm_make_request(KVM_REQ_TRIPLE_FAULT, vcpu);
		return 1;
	}
	vmcs12 = kmap(page);
	vmcs12->launch_state = 0;
	kunmap(page);
	nested_release_page(page);

	nested_free_vmcs02(vmx, vmptr);

	skip_emulated_instruction(vcpu);
	nested_vmx_succeed(vcpu);
	return 1;
}
