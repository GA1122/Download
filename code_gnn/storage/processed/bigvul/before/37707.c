static void create_pit_timer(struct kvm *kvm, u32 val, int is_period)
{
	struct kvm_kpit_state *ps = &kvm->arch.vpit->pit_state;
	s64 interval;

	if (!irqchip_in_kernel(kvm) || ps->flags & KVM_PIT_FLAGS_HPET_LEGACY)
		return;

	interval = muldiv64(val, NSEC_PER_SEC, KVM_PIT_FREQ);

	pr_debug("create pit timer, interval is %llu nsec\n", interval);

	 
	hrtimer_cancel(&ps->timer);
	flush_kthread_work(&ps->pit->expired);
	ps->period = interval;
	ps->is_periodic = is_period;

	ps->timer.function = pit_timer_fn;
	ps->kvm = ps->pit->kvm;

	atomic_set(&ps->pending, 0);
	ps->irq_ack = 1;

	 
	if (ps->is_periodic) {
		s64 min_period = min_timer_period_us * 1000LL;

		if (ps->period < min_period) {
			pr_info_ratelimited(
			    "kvm: requested %lld ns "
			    "i8254 timer period limited to %lld ns\n",
			    ps->period, min_period);
			ps->period = min_period;
		}
	}

	hrtimer_start(&ps->timer, ktime_add_ns(ktime_get(), interval),
		      HRTIMER_MODE_ABS);
}
