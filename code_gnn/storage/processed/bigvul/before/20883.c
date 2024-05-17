static void toggle_interruptibility(struct kvm_vcpu *vcpu, u32 mask)
{
	u32 int_shadow = kvm_x86_ops->get_interrupt_shadow(vcpu, mask);
	 
	if (!(int_shadow & mask))
		kvm_x86_ops->set_interrupt_shadow(vcpu, mask);
}
