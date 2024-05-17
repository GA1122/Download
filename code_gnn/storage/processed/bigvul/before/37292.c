static void vmx_set_apic_access_page_addr(struct kvm_vcpu *vcpu, hpa_t hpa)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	 
	if (!is_guest_mode(vcpu) ||
	    !nested_cpu_has2(vmx->nested.current_vmcs12,
			     SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES))
		vmcs_write64(APIC_ACCESS_ADDR, hpa);
}
