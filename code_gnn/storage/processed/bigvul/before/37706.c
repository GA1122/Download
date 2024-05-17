static s64 __kpit_elapsed(struct kvm *kvm)
{
	s64 elapsed;
	ktime_t remaining;
	struct kvm_kpit_state *ps = &kvm->arch.vpit->pit_state;

	if (!ps->period)
		return 0;

	 
	remaining = hrtimer_get_remaining(&ps->timer);
	elapsed = ps->period - ktime_to_ns(remaining);

	return elapsed;
}
