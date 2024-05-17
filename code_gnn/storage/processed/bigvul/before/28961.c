void kvm_arch_vcpu_load(struct kvm_vcpu *vcpu, int cpu)
{
	vcpu->cpu = cpu;
	vcpu->arch.host_cpu_context = this_cpu_ptr(kvm_host_cpu_state);

	 
	if (cpumask_test_and_clear_cpu(cpu, &vcpu->arch.require_dcache_flush))
		flush_cache_all();  

	kvm_arm_set_running_vcpu(vcpu);
}
