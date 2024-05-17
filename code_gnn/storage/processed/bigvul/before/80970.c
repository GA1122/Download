static void load_vmcs12_mmu_host_state(struct kvm_vcpu *vcpu,
			struct vmcs12 *vmcs12)
{
	u32 entry_failure_code;

	nested_ept_uninit_mmu_context(vcpu);

	 
	if (nested_vmx_load_cr3(vcpu, vmcs12->host_cr3, false, &entry_failure_code))
		nested_vmx_abort(vcpu, VMX_ABORT_LOAD_HOST_PDPTE_FAIL);

	if (!enable_ept)
		vcpu->arch.walk_mmu->inject_page_fault = kvm_inject_page_fault;
}