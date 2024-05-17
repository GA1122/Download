static int handle_ipi(struct kvm_vcpu *vcpu, struct kvm_run *kvm_run)
{
	struct exit_ctl_data *p = kvm_get_exit_data(vcpu);
	struct kvm_vcpu *target_vcpu;
	struct kvm_pt_regs *regs;
	union ia64_ipi_a addr = p->u.ipi_data.addr;
	union ia64_ipi_d data = p->u.ipi_data.data;

	target_vcpu = lid_to_vcpu(vcpu->kvm, addr.id, addr.eid);
	if (!target_vcpu)
		return handle_vm_error(vcpu, kvm_run);

	if (!target_vcpu->arch.launched) {
		regs = vcpu_regs(target_vcpu);

		regs->cr_iip = vcpu->kvm->arch.rdv_sal_data.boot_ip;
		regs->r1 = vcpu->kvm->arch.rdv_sal_data.boot_gp;

		target_vcpu->arch.mp_state = KVM_MP_STATE_RUNNABLE;
		if (waitqueue_active(&target_vcpu->wq))
			wake_up_interruptible(&target_vcpu->wq);
	} else {
		vcpu_deliver_ipi(target_vcpu, data.dm, data.vector);
		if (target_vcpu != vcpu)
			kvm_vcpu_kick(target_vcpu);
	}

	return 1;
}
