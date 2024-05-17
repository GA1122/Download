static void pit_do_work(struct kthread_work *work)
{
	struct kvm_pit *pit = container_of(work, struct kvm_pit, expired);
	struct kvm *kvm = pit->kvm;
	struct kvm_vcpu *vcpu;
	int i;
	struct kvm_kpit_state *ps = &pit->pit_state;
	int inject = 0;

	 
	spin_lock(&ps->inject_lock);
	if (ps->irq_ack) {
		ps->irq_ack = 0;
		inject = 1;
	}
	spin_unlock(&ps->inject_lock);
	if (inject) {
		kvm_set_irq(kvm, kvm->arch.vpit->irq_source_id, 0, 1, false);
		kvm_set_irq(kvm, kvm->arch.vpit->irq_source_id, 0, 0, false);

		 
		if (kvm->arch.vapics_in_nmi_mode > 0)
			kvm_for_each_vcpu(i, vcpu, kvm)
				kvm_apic_nmi_wd_deliver(vcpu);
	}
}
