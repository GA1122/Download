static void vmx_set_apic_access_page_addr(struct kvm_vcpu *vcpu, hpa_t hpa)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	 
	if (!is_guest_mode(vcpu) ||
	    !nested_cpu_has2(get_vmcs12(&vmx->vcpu),
			     SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES)) {
		vmcs_write64(APIC_ACCESS_ADDR, hpa);
		vmx_flush_tlb_ept_only(vcpu);
	}
}
