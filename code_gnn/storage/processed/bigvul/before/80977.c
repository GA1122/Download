static int nested_ept_init_mmu_context(struct kvm_vcpu *vcpu)
{
	WARN_ON(mmu_is_nested(vcpu));
	if (!valid_ept_address(vcpu, nested_ept_get_cr3(vcpu)))
		return 1;

	kvm_mmu_unload(vcpu);
	kvm_init_shadow_ept_mmu(vcpu,
			to_vmx(vcpu)->nested.msrs.ept_caps &
			VMX_EPT_EXECUTE_ONLY_BIT,
			nested_ept_ad_enabled(vcpu));
	vcpu->arch.mmu.set_cr3           = vmx_set_cr3;
	vcpu->arch.mmu.get_cr3           = nested_ept_get_cr3;
	vcpu->arch.mmu.inject_page_fault = nested_ept_inject_page_fault;

	vcpu->arch.walk_mmu              = &vcpu->arch.nested_mmu;
	return 0;
}