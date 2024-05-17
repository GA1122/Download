static int kvm_vcpu_ioctl_x86_set_xsave(struct kvm_vcpu *vcpu,
					struct kvm_xsave *guest_xsave)
{
	u64 xstate_bv =
		*(u64 *)&guest_xsave->region[XSAVE_HDR_OFFSET / sizeof(u32)];

	if (cpu_has_xsave) {
		 
		if (xstate_bv & ~KVM_SUPPORTED_XCR0)
			return -EINVAL;
		if (xstate_bv & ~host_xcr0)
			return -EINVAL;
		memcpy(&vcpu->arch.guest_fpu.state->xsave,
			guest_xsave->region, vcpu->arch.guest_xstate_size);
	} else {
		if (xstate_bv & ~XSTATE_FPSSE)
			return -EINVAL;
		memcpy(&vcpu->arch.guest_fpu.state->fxsave,
			guest_xsave->region, sizeof(struct i387_fxsave_struct));
	}
	return 0;
}