void kvm_load_guest_fpu(struct kvm_vcpu *vcpu)
{
	if (vcpu->guest_fpu_loaded)
		return;

	 
	kvm_put_guest_xcr0(vcpu);
	vcpu->guest_fpu_loaded = 1;
	unlazy_fpu(current);
	fpu_restore_checking(&vcpu->arch.guest_fpu);
	trace_kvm_fpu(1);
}
