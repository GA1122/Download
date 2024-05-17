void kvm_arch_vcpu_load(struct kvm_vcpu *vcpu, int cpu)
{
	if (cpu != vcpu->cpu) {
		vcpu->cpu = cpu;
		if (vcpu->arch.ht_active)
			kvm_migrate_hlt_timer(vcpu);
	}
}
