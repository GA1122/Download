void kvm_sigset_activate(struct kvm_vcpu *vcpu)
{
	if (!vcpu->sigset_active)
		return;

	 
	sigprocmask(SIG_SETMASK, &vcpu->sigset, &current->real_blocked);
}
