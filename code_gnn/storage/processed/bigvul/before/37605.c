static void walk_shadow_page_lockless_end(struct kvm_vcpu *vcpu)
{
	 
	smp_mb();
	vcpu->mode = OUTSIDE_GUEST_MODE;
	local_irq_enable();
}
