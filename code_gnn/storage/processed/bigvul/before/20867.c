static inline u64 nsec_to_cycles(struct kvm_vcpu *vcpu, u64 nsec)
{
	u64 ret;

	WARN_ON(preemptible());
	if (kvm_tsc_changes_freq())
		printk_once(KERN_WARNING
		 "kvm: unreliable cycle conversion on adjustable rate TSC\n");
	ret = nsec * vcpu_tsc_khz(vcpu);
	do_div(ret, USEC_PER_SEC);
	return ret;
}
