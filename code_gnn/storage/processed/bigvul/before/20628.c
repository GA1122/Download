static void kvm_migrate_hlt_timer(struct kvm_vcpu *vcpu)
{
	struct hrtimer *p_ht = &vcpu->arch.hlt_timer;

	if (hrtimer_cancel(p_ht))
		hrtimer_start_expires(p_ht, HRTIMER_MODE_ABS);
}
