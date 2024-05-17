static void vmx_vcpu_pi_load(struct kvm_vcpu *vcpu, int cpu)
{
	struct pi_desc *pi_desc = vcpu_to_pi_desc(vcpu);
	struct pi_desc old, new;
	unsigned int dest;

	 
	if (!pi_test_sn(pi_desc) && vcpu->cpu == cpu)
		return;

	 
	if (pi_desc->nv == POSTED_INTR_WAKEUP_VECTOR ||
	    vcpu->cpu == cpu) {
		pi_clear_sn(pi_desc);
		return;
	}

	 
	do {
		old.control = new.control = pi_desc->control;

		dest = cpu_physical_id(cpu);

		if (x2apic_enabled())
			new.ndst = dest;
		else
			new.ndst = (dest << 8) & 0xFF00;

		new.sn = 0;
	} while (cmpxchg64(&pi_desc->control, old.control,
			   new.control) != old.control);
}
