static int wrmsr_interception(struct vcpu_svm *svm)
{
	struct msr_data msr;
	u32 ecx = kvm_register_read(&svm->vcpu, VCPU_REGS_RCX);
	u64 data = kvm_read_edx_eax(&svm->vcpu);

	msr.data = data;
	msr.index = ecx;
	msr.host_initiated = false;

	svm->next_rip = kvm_rip_read(&svm->vcpu) + 2;
	if (kvm_set_msr(&svm->vcpu, &msr)) {
		trace_kvm_msr_write_ex(ecx, data);
		kvm_inject_gp(&svm->vcpu, 0);
	} else {
		trace_kvm_msr_write(ecx, data);
		skip_emulated_instruction(&svm->vcpu);
	}
	return 1;
}