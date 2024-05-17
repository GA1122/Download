int kvm_emulate_halt(struct kvm_vcpu *vcpu)
{

	ktime_t kt;
	long itc_diff;
	unsigned long vcpu_now_itc;
	unsigned long expires;
	struct hrtimer *p_ht = &vcpu->arch.hlt_timer;
	unsigned long cyc_per_usec = local_cpu_data->cyc_per_usec;
	struct vpd *vpd = to_host(vcpu->kvm, vcpu->arch.vpd);

	if (irqchip_in_kernel(vcpu->kvm)) {

		vcpu_now_itc = kvm_get_itc(vcpu) + vcpu->arch.itc_offset;

		if (time_after(vcpu_now_itc, vpd->itm)) {
			vcpu->arch.timer_check = 1;
			return 1;
		}
		itc_diff = vpd->itm - vcpu_now_itc;
		if (itc_diff < 0)
			itc_diff = -itc_diff;

		expires = div64_u64(itc_diff, cyc_per_usec);
		kt = ktime_set(0, 1000 * expires);

		vcpu->arch.ht_active = 1;
		hrtimer_start(p_ht, kt, HRTIMER_MODE_ABS);

		vcpu->arch.mp_state = KVM_MP_STATE_HALTED;
		kvm_vcpu_block(vcpu);
		hrtimer_cancel(p_ht);
		vcpu->arch.ht_active = 0;

		if (test_and_clear_bit(KVM_REQ_UNHALT, &vcpu->requests) ||
				kvm_cpu_has_pending_timer(vcpu))
			if (vcpu->arch.mp_state == KVM_MP_STATE_HALTED)
				vcpu->arch.mp_state = KVM_MP_STATE_RUNNABLE;

		if (vcpu->arch.mp_state != KVM_MP_STATE_RUNNABLE)
			return -EINTR;
		return 1;
	} else {
		printk(KERN_ERR"kvm: Unsupported userspace halt!");
		return 0;
	}
}
