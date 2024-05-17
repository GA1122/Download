int kvm_arch_vcpu_ioctl_get_stack(struct kvm_vcpu *vcpu,
				  struct kvm_ia64_vcpu_stack *stack)
{
	memcpy(stack, vcpu, sizeof(struct kvm_ia64_vcpu_stack));
	return 0;
}
