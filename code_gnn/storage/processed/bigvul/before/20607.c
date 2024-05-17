int kvm_arch_vcpu_ioctl_set_stack(struct kvm_vcpu *vcpu,
				  struct kvm_ia64_vcpu_stack *stack)
{
	memcpy(vcpu + 1, &stack->stack[0] + sizeof(struct kvm_vcpu),
	       sizeof(struct kvm_ia64_vcpu_stack) - sizeof(struct kvm_vcpu));

	vcpu->arch.exit_data = ((struct kvm_vcpu *)stack)->arch.exit_data;
	return 0;
}
