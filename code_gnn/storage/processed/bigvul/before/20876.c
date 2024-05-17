static bool retry_instruction(struct x86_emulate_ctxt *ctxt,
			      unsigned long cr2,  int emulation_type)
{
	struct kvm_vcpu *vcpu = emul_to_vcpu(ctxt);
	unsigned long last_retry_eip, last_retry_addr, gpa = cr2;

	last_retry_eip = vcpu->arch.last_retry_eip;
	last_retry_addr = vcpu->arch.last_retry_addr;

	 
	vcpu->arch.last_retry_eip = vcpu->arch.last_retry_addr = 0;

	if (!(emulation_type & EMULTYPE_RETRY))
		return false;

	if (x86_page_table_writing_insn(ctxt))
		return false;

	if (ctxt->eip == last_retry_eip && last_retry_addr == cr2)
		return false;

	vcpu->arch.last_retry_eip = ctxt->eip;
	vcpu->arch.last_retry_addr = cr2;

	if (!vcpu->arch.mmu.direct_map)
		gpa = kvm_mmu_gva_to_gpa_write(vcpu, cr2, NULL);

	kvm_mmu_unprotect_page(vcpu->kvm, gpa >> PAGE_SHIFT);

	return true;
}
