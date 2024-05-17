static s64 __kpit_elapsed(struct kvm *kvm)
{
	s64 elapsed;
	ktime_t remaining;
	struct kvm_kpit_state *ps = &kvm->arch.vpit->pit_state;

	if (!ps->pit_timer.period)
		return 0;

	 
	remaining = hrtimer_get_remaining(&ps->pit_timer.timer);
	elapsed = ps->pit_timer.period - ktime_to_ns(remaining);
	elapsed = mod_64(elapsed, ps->pit_timer.period);

	return elapsed;
}
