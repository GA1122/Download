static int kvmclock_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
				     void *data)
{
	struct cpufreq_freqs *freq = data;
	struct kvm *kvm;
	struct kvm_vcpu *vcpu;
	int i, send_ipi = 0;

	 

	if (val == CPUFREQ_PRECHANGE && freq->old > freq->new)
		return 0;
	if (val == CPUFREQ_POSTCHANGE && freq->old < freq->new)
		return 0;

	smp_call_function_single(freq->cpu, tsc_khz_changed, freq, 1);

	raw_spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
		kvm_for_each_vcpu(i, vcpu, kvm) {
			if (vcpu->cpu != freq->cpu)
				continue;
			kvm_make_request(KVM_REQ_CLOCK_UPDATE, vcpu);
			if (vcpu->cpu != smp_processor_id())
				send_ipi = 1;
		}
	}
	raw_spin_unlock(&kvm_lock);

	if (freq->old < freq->new && send_ipi) {
		 
		smp_call_function_single(freq->cpu, tsc_khz_changed, freq, 1);
	}
	return 0;
}
