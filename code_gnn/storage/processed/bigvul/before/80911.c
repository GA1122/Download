static void __pi_post_block(struct kvm_vcpu *vcpu)
{
	struct pi_desc *pi_desc = vcpu_to_pi_desc(vcpu);
	struct pi_desc old, new;
	unsigned int dest;

	do {
		old.control = new.control = pi_desc->control;
		WARN(old.nv != POSTED_INTR_WAKEUP_VECTOR,
		     "Wakeup handler not enabled while the VCPU is blocked\n");

		dest = cpu_physical_id(vcpu->cpu);

		if (x2apic_enabled())
			new.ndst = dest;
		else
			new.ndst = (dest << 8) & 0xFF00;

		 
		new.nv = POSTED_INTR_VECTOR;
	} while (cmpxchg64(&pi_desc->control, old.control,
			   new.control) != old.control);

	if (!WARN_ON_ONCE(vcpu->pre_pcpu == -1)) {
		spin_lock(&per_cpu(blocked_vcpu_on_cpu_lock, vcpu->pre_pcpu));
		list_del(&vcpu->blocked_vcpu_list);
		spin_unlock(&per_cpu(blocked_vcpu_on_cpu_lock, vcpu->pre_pcpu));
		vcpu->pre_pcpu = -1;
	}
}