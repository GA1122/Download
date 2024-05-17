int kvm_arch_vcpu_ioctl_set_guest_debug(struct kvm_vcpu *vcpu,
					struct kvm_guest_debug *dbg)
{
	int ret = 0;

	trace_kvm_set_guest_debug(vcpu, dbg->control);

	if (dbg->control & ~KVM_GUESTDBG_VALID_MASK) {
		ret = -EINVAL;
		goto out;
	}

	if (dbg->control & KVM_GUESTDBG_ENABLE) {
		vcpu->guest_debug = dbg->control;

		 
		if (vcpu->guest_debug & KVM_GUESTDBG_USE_HW) {
			vcpu->arch.external_debug_state = dbg->arch;
		}

	} else {
		 
		vcpu->guest_debug = 0;
	}

out:
	return ret;
}