static bool reexecute_instruction(struct kvm_vcpu *vcpu, gva_t gva)
{
	gpa_t gpa;

	if (tdp_enabled)
		return false;

	 
	if (kvm_mmu_unprotect_page_virt(vcpu, gva))
		return true;

	gpa = kvm_mmu_gva_to_gpa_system(vcpu, gva, NULL);

	if (gpa == UNMAPPED_GVA)
		return true;  

	if (!kvm_is_error_hva(gfn_to_hva(vcpu->kvm, gpa >> PAGE_SHIFT)))
		return true;

	return false;
}