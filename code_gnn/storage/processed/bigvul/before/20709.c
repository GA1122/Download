void kvm_after_handle_nmi(struct kvm_vcpu *vcpu)
{
	__this_cpu_write(current_vcpu, NULL);
}