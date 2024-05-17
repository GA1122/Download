static void shared_msr_update(unsigned slot, u32 msr)
{
	struct kvm_shared_msrs *smsr;
	u64 value;

	smsr = &__get_cpu_var(shared_msrs);
	 
	if (slot >= shared_msrs_global.nr) {
		printk(KERN_ERR "kvm: invalid MSR slot!");
		return;
	}
	rdmsrl_safe(msr, &value);
	smsr->values[slot].host = value;
	smsr->values[slot].curr = value;
}