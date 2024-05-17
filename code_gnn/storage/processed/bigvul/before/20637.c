static int kvm_vcpu_pre_transition(struct kvm_vcpu *vcpu)
{
	unsigned long psr;
	int r;
	int cpu = smp_processor_id();

	if (vcpu->arch.last_run_cpu != cpu ||
			per_cpu(last_vcpu, cpu) != vcpu) {
		per_cpu(last_vcpu, cpu) = vcpu;
		vcpu->arch.last_run_cpu = cpu;
		kvm_flush_tlb_all();
	}

	vcpu->arch.host_rr6 = ia64_get_rr(RR6);
	vti_set_rr6(vcpu->arch.vmm_rr);
	local_irq_save(psr);
	r = kvm_insert_vmm_mapping(vcpu);
	local_irq_restore(psr);
	return r;
}
