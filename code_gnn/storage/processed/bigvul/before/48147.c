static void vmx_vcpu_pi_load(struct kvm_vcpu *vcpu, int cpu)
{
	struct pi_desc *pi_desc = vcpu_to_pi_desc(vcpu);
	struct pi_desc old, new;
	unsigned int dest;

	if (!kvm_arch_has_assigned_device(vcpu->kvm) ||
		!irq_remapping_cap(IRQ_POSTING_CAP)  ||
		!kvm_vcpu_apicv_active(vcpu))
		return;

	do {
		old.control = new.control = pi_desc->control;

		 
		if (pi_desc->nv != POSTED_INTR_WAKEUP_VECTOR) {
			if (vcpu->cpu != cpu) {
				dest = cpu_physical_id(cpu);

				if (x2apic_enabled())
					new.ndst = dest;
				else
					new.ndst = (dest << 8) & 0xFF00;
			}

			 
			new.nv = POSTED_INTR_VECTOR;
		}

		 
		new.sn = 0;
	} while (cmpxchg(&pi_desc->control, old.control,
			new.control) != old.control);
}
