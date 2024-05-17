static int handle_sal_call(struct kvm_vcpu *vcpu, struct kvm_run *kvm_run)
{
	struct exit_ctl_data *p;

	p = kvm_get_exit_data(vcpu);

	if (p->exit_reason == EXIT_REASON_SAL_CALL) {
		kvm_sal_emul(vcpu);
		return 1;
	} else {
		kvm_run->exit_reason = KVM_EXIT_UNKNOWN;
		kvm_run->hw.hardware_exit_reason = 3;
		return 0;
	}

}
