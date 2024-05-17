static bool reexecute_instruction(struct kvm_vcpu *vcpu, gva_t cr2,
				  bool write_fault_to_shadow_pgtable,
				  int emulation_type)
{
	gpa_t gpa = cr2;
	pfn_t pfn;

	if (emulation_type & EMULTYPE_NO_REEXECUTE)
		return false;

	if (!vcpu->arch.mmu.direct_map) {
		 
		gpa = kvm_mmu_gva_to_gpa_write(vcpu, cr2, NULL);

		 
		if (gpa == UNMAPPED_GVA)
			return true;
	}

	 
	pfn = gfn_to_pfn(vcpu->kvm, gpa_to_gfn(gpa));

	 
	if (is_error_noslot_pfn(pfn))
		return false;

	kvm_release_pfn_clean(pfn);

	 
	if (vcpu->arch.mmu.direct_map) {
		unsigned int indirect_shadow_pages;

		spin_lock(&vcpu->kvm->mmu_lock);
		indirect_shadow_pages = vcpu->kvm->arch.indirect_shadow_pages;
		spin_unlock(&vcpu->kvm->mmu_lock);

		if (indirect_shadow_pages)
			kvm_mmu_unprotect_page(vcpu->kvm, gpa_to_gfn(gpa));

		return true;
	}

	 
	kvm_mmu_unprotect_page(vcpu->kvm, gpa_to_gfn(gpa));

	 
	return !write_fault_to_shadow_pgtable;
}
