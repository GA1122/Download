static void load_xsave(struct kvm_vcpu *vcpu, u8 *src)
{
	struct xregs_state *xsave = &vcpu->arch.guest_fpu.state.xsave;
	u64 xstate_bv = *(u64 *)(src + XSAVE_HDR_OFFSET);
	u64 valid;

	 
	memcpy(xsave, src, XSAVE_HDR_OFFSET);

	 
	xsave->header.xfeatures = xstate_bv;
	if (cpu_has_xsaves)
		xsave->header.xcomp_bv = host_xcr0 | XSTATE_COMPACTION_ENABLED;

	 
	valid = xstate_bv & ~XFEATURE_MASK_FPSSE;
	while (valid) {
		u64 feature = valid & -valid;
		int index = fls64(feature) - 1;
		void *dest = get_xsave_addr(xsave, feature);

		if (dest) {
			u32 size, offset, ecx, edx;
			cpuid_count(XSTATE_CPUID, index,
				    &size, &offset, &ecx, &edx);
			memcpy(dest, src + offset, size);
		}

		valid -= feature;
	}
}
