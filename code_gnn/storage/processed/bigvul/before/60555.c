int kvmppc_prepare_to_enter(struct kvm_vcpu *vcpu)
{
	int r;

	WARN_ON(irqs_disabled());
	hard_irq_disable();

	while (true) {
		if (need_resched()) {
			local_irq_enable();
			cond_resched();
			hard_irq_disable();
			continue;
		}

		if (signal_pending(current)) {
			kvmppc_account_exit(vcpu, SIGNAL_EXITS);
			vcpu->run->exit_reason = KVM_EXIT_INTR;
			r = -EINTR;
			break;
		}

		vcpu->mode = IN_GUEST_MODE;

		 
		smp_mb();

		if (kvm_request_pending(vcpu)) {
			 
			local_irq_enable();
			trace_kvm_check_requests(vcpu);
			r = kvmppc_core_check_requests(vcpu);
			hard_irq_disable();
			if (r > 0)
				continue;
			break;
		}

		if (kvmppc_core_prepare_to_enter(vcpu)) {
			 
			continue;
		}

		guest_enter_irqoff();
		return 1;
	}

	 
	local_irq_enable();
	return r;
}
