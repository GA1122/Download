static void __inject_pit_timer_intr(struct kvm *kvm)
{
	struct kvm_vcpu *vcpu;
	int i;

	kvm_set_irq(kvm, kvm->arch.vpit->irq_source_id, 0, 1);
	kvm_set_irq(kvm, kvm->arch.vpit->irq_source_id, 0, 0);

	 
	if (kvm->arch.vapics_in_nmi_mode > 0)
		kvm_for_each_vcpu(i, vcpu, kvm)
			kvm_apic_nmi_wd_deliver(vcpu);
}
