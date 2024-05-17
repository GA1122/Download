static int handle_global_purge(struct kvm_vcpu *vcpu, struct kvm_run *kvm_run)
{
	struct exit_ctl_data *p = kvm_get_exit_data(vcpu);
	struct kvm *kvm = vcpu->kvm;
	struct call_data call_data;
	int i;
	struct kvm_vcpu *vcpui;

	call_data.ptc_g_data = p->u.ptc_g_data;

	kvm_for_each_vcpu(i, vcpui, kvm) {
		if (vcpui->arch.mp_state == KVM_MP_STATE_UNINITIALIZED ||
				vcpu == vcpui)
			continue;

		if (waitqueue_active(&vcpui->wq))
			wake_up_interruptible(&vcpui->wq);

		if (vcpui->cpu != -1) {
			call_data.vcpu = vcpui;
			smp_call_function_single(vcpui->cpu,
					vcpu_global_purge, &call_data, 1);
		} else
			printk(KERN_WARNING"kvm: Uninit vcpu received ipi!\n");

	}
	return 1;
}
