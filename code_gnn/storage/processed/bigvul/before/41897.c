static int invlpga_interception(struct vcpu_svm *svm)
{
	struct kvm_vcpu *vcpu = &svm->vcpu;

	trace_kvm_invlpga(svm->vmcb->save.rip, kvm_register_read(&svm->vcpu, VCPU_REGS_RCX),
			  kvm_register_read(&svm->vcpu, VCPU_REGS_RAX));

	 
	kvm_mmu_invlpg(vcpu, kvm_register_read(&svm->vcpu, VCPU_REGS_RAX));

	svm->next_rip = kvm_rip_read(&svm->vcpu) + 3;
	skip_emulated_instruction(&svm->vcpu);
	return 1;
}