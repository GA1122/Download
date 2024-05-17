static inline bool nested_vmx_prepare_msr_bitmap(struct kvm_vcpu *vcpu,
						 struct vmcs12 *vmcs12)
{
	int msr;
	struct page *page;
	unsigned long *msr_bitmap_l1;
	unsigned long *msr_bitmap_l0 = to_vmx(vcpu)->nested.vmcs02.msr_bitmap;
	 
	bool pred_cmd = !msr_write_intercepted_l01(vcpu, MSR_IA32_PRED_CMD);
	bool spec_ctrl = !msr_write_intercepted_l01(vcpu, MSR_IA32_SPEC_CTRL);

	 
	if (!cpu_has_vmx_msr_bitmap() ||
	    !nested_cpu_has(vmcs12, CPU_BASED_USE_MSR_BITMAPS))
		return false;

	if (!nested_cpu_has_virt_x2apic_mode(vmcs12) &&
	    !pred_cmd && !spec_ctrl)
		return false;

	page = kvm_vcpu_gpa_to_page(vcpu, vmcs12->msr_bitmap);
	if (is_error_page(page))
		return false;

	msr_bitmap_l1 = (unsigned long *)kmap(page);
	if (nested_cpu_has_apic_reg_virt(vmcs12)) {
		 
		for (msr = 0x800; msr <= 0x8ff; msr += BITS_PER_LONG) {
			unsigned word = msr / BITS_PER_LONG;
			msr_bitmap_l0[word] = msr_bitmap_l1[word];
			msr_bitmap_l0[word + (0x800 / sizeof(long))] = ~0;
		}
	} else {
		for (msr = 0x800; msr <= 0x8ff; msr += BITS_PER_LONG) {
			unsigned word = msr / BITS_PER_LONG;
			msr_bitmap_l0[word] = ~0;
			msr_bitmap_l0[word + (0x800 / sizeof(long))] = ~0;
		}
	}

	nested_vmx_disable_intercept_for_msr(
		msr_bitmap_l1, msr_bitmap_l0,
		X2APIC_MSR(APIC_TASKPRI),
		MSR_TYPE_W);

	if (nested_cpu_has_vid(vmcs12)) {
		nested_vmx_disable_intercept_for_msr(
			msr_bitmap_l1, msr_bitmap_l0,
			X2APIC_MSR(APIC_EOI),
			MSR_TYPE_W);
		nested_vmx_disable_intercept_for_msr(
			msr_bitmap_l1, msr_bitmap_l0,
			X2APIC_MSR(APIC_SELF_IPI),
			MSR_TYPE_W);
	}

	if (spec_ctrl)
		nested_vmx_disable_intercept_for_msr(
					msr_bitmap_l1, msr_bitmap_l0,
					MSR_IA32_SPEC_CTRL,
					MSR_TYPE_R | MSR_TYPE_W);

	if (pred_cmd)
		nested_vmx_disable_intercept_for_msr(
					msr_bitmap_l1, msr_bitmap_l0,
					MSR_IA32_PRED_CMD,
					MSR_TYPE_W);

	kunmap(page);
	kvm_release_page_clean(page);

	return true;
}
