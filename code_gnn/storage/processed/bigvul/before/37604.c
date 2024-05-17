static void walk_shadow_page_lockless_begin(struct kvm_vcpu *vcpu)
{
	 
	local_irq_disable();
	vcpu->mode = READING_SHADOW_PAGE_TABLES;
	 
	smp_mb();
}
