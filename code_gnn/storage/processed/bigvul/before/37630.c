static int handle_set_cr0(struct kvm_vcpu *vcpu, unsigned long val)
{
	if (is_guest_mode(vcpu)) {
		struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
		unsigned long orig_val = val;

		 
		val = (val & ~vmcs12->cr0_guest_host_mask) |
			(vmcs12->guest_cr0 & vmcs12->cr0_guest_host_mask);

		 
		if ((val & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON)
			return 1;

		if (kvm_set_cr0(vcpu, val))
			return 1;
		vmcs_writel(CR0_READ_SHADOW, orig_val);
		return 0;
	} else {
		if (to_vmx(vcpu)->nested.vmxon &&
		    ((val & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON))
			return 1;
		return kvm_set_cr0(vcpu, val);
	}
}
