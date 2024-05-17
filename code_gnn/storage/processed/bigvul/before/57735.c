void kvm_load_guest_fpu(struct kvm_vcpu *vcpu)
{
	if (vcpu->guest_fpu_loaded)
		return;

	 
	kvm_put_guest_xcr0(vcpu);
	vcpu->guest_fpu_loaded = 1;
	__kernel_fpu_begin();
	__copy_kernel_to_fpregs(&vcpu->arch.guest_fpu.state);
	trace_kvm_fpu(1);
}
