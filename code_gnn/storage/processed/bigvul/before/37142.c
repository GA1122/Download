static bool nested_get_vmcs12_pages(struct kvm_vcpu *vcpu,
					struct vmcs12 *vmcs12)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);

	if (nested_cpu_has2(vmcs12, SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES)) {
		 
		if (!PAGE_ALIGNED(vmcs12->apic_access_addr))
			return false;

		 
		if (vmx->nested.apic_access_page)  
			nested_release_page(vmx->nested.apic_access_page);
		vmx->nested.apic_access_page =
			nested_get_page(vcpu, vmcs12->apic_access_addr);
	}

	if (nested_cpu_has(vmcs12, CPU_BASED_TPR_SHADOW)) {
		 
		if (!PAGE_ALIGNED(vmcs12->virtual_apic_page_addr))
			return false;

		if (vmx->nested.virtual_apic_page)  
			nested_release_page(vmx->nested.virtual_apic_page);
		vmx->nested.virtual_apic_page =
			nested_get_page(vcpu, vmcs12->virtual_apic_page_addr);

		 
		if (!vmx->nested.virtual_apic_page)
			return false;
	}

	return true;
}
