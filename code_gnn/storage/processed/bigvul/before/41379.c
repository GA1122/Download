int kvm_fix_hypercall(struct kvm_vcpu *vcpu)
{
	char instruction[3];
	unsigned long rip = kvm_rip_read(vcpu);

	 
	kvm_mmu_zap_all(vcpu->kvm);

	kvm_x86_ops->patch_hypercall(vcpu, instruction);

	return emulator_write_emulated(rip, instruction, 3, NULL, vcpu);
}
