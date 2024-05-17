vmcs12_guest_cr0(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
{
	return
	 	(vmcs_readl(GUEST_CR0) & vcpu->arch.cr0_guest_owned_bits) |
	 	(vmcs12->guest_cr0 & vmcs12->cr0_guest_host_mask) |
	 	(vmcs_readl(CR0_READ_SHADOW) & ~(vmcs12->cr0_guest_host_mask |
			vcpu->arch.cr0_guest_owned_bits));
}
