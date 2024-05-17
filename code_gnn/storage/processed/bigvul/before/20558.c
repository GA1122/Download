static int __vcpu_run(struct kvm_vcpu *vcpu, struct kvm_run *kvm_run)
{
	union context *host_ctx, *guest_ctx;
	int r, idx;

	idx = srcu_read_lock(&vcpu->kvm->srcu);

again:
	if (signal_pending(current)) {
		r = -EINTR;
		kvm_run->exit_reason = KVM_EXIT_INTR;
		goto out;
	}

	preempt_disable();
	local_irq_disable();

	 
	host_ctx = kvm_get_host_context(vcpu);
	guest_ctx = kvm_get_guest_context(vcpu);

	clear_bit(KVM_REQ_KICK, &vcpu->requests);

	r = kvm_vcpu_pre_transition(vcpu);
	if (r < 0)
		goto vcpu_run_fail;

	srcu_read_unlock(&vcpu->kvm->srcu, idx);
	vcpu->mode = IN_GUEST_MODE;
	kvm_guest_enter();

	 
	kvm_vmm_info->tramp_entry(host_ctx, guest_ctx);

	kvm_vcpu_post_transition(vcpu);

	vcpu->arch.launched = 1;
	set_bit(KVM_REQ_KICK, &vcpu->requests);
	local_irq_enable();

	 
	barrier();
	kvm_guest_exit();
	vcpu->mode = OUTSIDE_GUEST_MODE;
	preempt_enable();

	idx = srcu_read_lock(&vcpu->kvm->srcu);

	r = kvm_handle_exit(kvm_run, vcpu);

	if (r > 0) {
		if (!need_resched())
			goto again;
	}

out:
	srcu_read_unlock(&vcpu->kvm->srcu, idx);
	if (r > 0) {
		kvm_resched(vcpu);
		idx = srcu_read_lock(&vcpu->kvm->srcu);
		goto again;
	}

	return r;

vcpu_run_fail:
	local_irq_enable();
	preempt_enable();
	kvm_run->exit_reason = KVM_EXIT_FAIL_ENTRY;
	goto out;
}
