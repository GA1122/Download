static void vmx_fpu_deactivate(struct kvm_vcpu *vcpu)
{
	 
	vmx_decache_cr0_guest_bits(vcpu);
	vmcs_set_bits(GUEST_CR0, X86_CR0_TS | X86_CR0_MP);
	update_exception_bitmap(vcpu);
	vcpu->arch.cr0_guest_owned_bits = 0;
	vmcs_writel(CR0_GUEST_HOST_MASK, ~vcpu->arch.cr0_guest_owned_bits);
	if (is_guest_mode(vcpu)) {
		 
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		vmcs12->guest_cr0 = (vmcs12->guest_cr0 & ~X86_CR0_TS) |
			(vcpu->arch.cr0 & X86_CR0_TS);
		vmcs_writel(CR0_READ_SHADOW, nested_read_cr0(vmcs12));
	} else
		vmcs_writel(CR0_READ_SHADOW, vcpu->arch.cr0);
}
