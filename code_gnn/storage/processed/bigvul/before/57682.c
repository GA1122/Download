static void fill_xsave(u8 *dest, struct kvm_vcpu *vcpu)
{
	struct xregs_state *xsave = &vcpu->arch.guest_fpu.state.xsave;
	u64 xstate_bv = xsave->header.xfeatures;
	u64 valid;

	 
	memcpy(dest, xsave, XSAVE_HDR_OFFSET);

	 
	*(u64 *)(dest + XSAVE_HDR_OFFSET) = xstate_bv;

	 
	valid = xstate_bv & ~XFEATURE_MASK_FPSSE;
	while (valid) {
		u64 feature = valid & -valid;
		int index = fls64(feature) - 1;
		void *src = get_xsave_addr(xsave, feature);

		if (src) {
			u32 size, offset, ecx, edx;
			cpuid_count(XSTATE_CPUID, index,
				    &size, &offset, &ecx, &edx);
			memcpy(dest + offset, src, size);
		}

		valid -= feature;
	}
}
