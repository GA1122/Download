static void __kvm_ioapic_update_eoi(struct kvm_vcpu *vcpu,
			struct kvm_ioapic *ioapic, int vector, int trigger_mode)
{
	struct dest_map *dest_map = &ioapic->rtc_status.dest_map;
	struct kvm_lapic *apic = vcpu->arch.apic;
	int i;

	 
	if (test_bit(vcpu->vcpu_id, dest_map->map) &&
	    vector == dest_map->vectors[vcpu->vcpu_id])
		rtc_irq_eoi(ioapic, vcpu);

	for (i = 0; i < IOAPIC_NUM_PINS; i++) {
		union kvm_ioapic_redirect_entry *ent = &ioapic->redirtbl[i];

		if (ent->fields.vector != vector)
			continue;

		 
		spin_unlock(&ioapic->lock);
		kvm_notify_acked_irq(ioapic->kvm, KVM_IRQCHIP_IOAPIC, i);
		spin_lock(&ioapic->lock);

		if (trigger_mode != IOAPIC_LEVEL_TRIG ||
		    kvm_lapic_get_reg(apic, APIC_SPIV) & APIC_SPIV_DIRECTED_EOI)
			continue;

		ASSERT(ent->fields.trig_mode == IOAPIC_LEVEL_TRIG);
		ent->fields.remote_irr = 0;
		if (!ent->fields.mask && (ioapic->irr & (1 << i))) {
			++ioapic->irq_eoi[i];
			if (ioapic->irq_eoi[i] == IOAPIC_SUCCESSIVE_IRQ_MAX_COUNT) {
				 
				schedule_delayed_work(&ioapic->eoi_inject, HZ / 100);
				ioapic->irq_eoi[i] = 0;
				trace_kvm_ioapic_delayed_eoi_inj(ent->bits);
			} else {
				ioapic_service(ioapic, i, false);
			}
		} else {
			ioapic->irq_eoi[i] = 0;
		}
	}
}
