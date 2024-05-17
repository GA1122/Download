static void free_nested(struct vcpu_vmx *vmx)
{
	if (!vmx->nested.vmxon)
		return;

	vmx->nested.vmxon = false;
	free_vpid(vmx->nested.vpid02);
	nested_release_vmcs12(vmx);
	if (vmx->nested.msr_bitmap) {
		free_page((unsigned long)vmx->nested.msr_bitmap);
		vmx->nested.msr_bitmap = NULL;
	}
	if (enable_shadow_vmcs) {
		vmcs_clear(vmx->vmcs01.shadow_vmcs);
		free_vmcs(vmx->vmcs01.shadow_vmcs);
		vmx->vmcs01.shadow_vmcs = NULL;
	}
	kfree(vmx->nested.cached_vmcs12);
	 
	if (vmx->nested.apic_access_page) {
		nested_release_page(vmx->nested.apic_access_page);
		vmx->nested.apic_access_page = NULL;
	}
	if (vmx->nested.virtual_apic_page) {
		nested_release_page(vmx->nested.virtual_apic_page);
		vmx->nested.virtual_apic_page = NULL;
	}
	if (vmx->nested.pi_desc_page) {
		kunmap(vmx->nested.pi_desc_page);
		nested_release_page(vmx->nested.pi_desc_page);
		vmx->nested.pi_desc_page = NULL;
		vmx->nested.pi_desc = NULL;
	}

	nested_free_all_saved_vmcss(vmx);
}