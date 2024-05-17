static void pit_load_count(struct kvm *kvm, int channel, u32 val)
{
	struct kvm_kpit_state *ps = &kvm->arch.vpit->pit_state;

	WARN_ON(!mutex_is_locked(&ps->lock));

	pr_debug("load_count val is %d, channel is %d\n", val, channel);

	 
	if (val == 0)
		val = 0x10000;

	ps->channels[channel].count = val;

	if (channel != 0) {
		ps->channels[channel].count_load_time = ktime_get();
		return;
	}

	 
	switch (ps->channels[0].mode) {
	case 0:
	case 1:
         
	case 4:
		create_pit_timer(kvm, val, 0);
		break;
	case 2:
	case 3:
		create_pit_timer(kvm, val, 1);
		break;
	default:
		destroy_pit_timer(kvm->arch.vpit);
	}
}
