static int handle_pal_call(struct kvm_vcpu *vcpu, struct kvm_run *kvm_run)
{
	struct exit_ctl_data *p;

	p = kvm_get_exit_data(vcpu);

	if (p->exit_reason == EXIT_REASON_PAL_CALL)
		return kvm_pal_emul(vcpu, kvm_run);
	else {
		kvm_run->exit_reason = KVM_EXIT_UNKNOWN;
		kvm_run->hw.hardware_exit_reason = 2;
		return 0;
	}
}
