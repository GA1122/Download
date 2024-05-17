static inline bool nested_vmx_merge_msr_bitmap(struct kvm_vcpu *vcpu,
					       struct vmcs12 *vmcs12)
{
	int msr;
	struct page *page;
	unsigned long *msr_bitmap_l1;
	unsigned long *msr_bitmap_l0 = to_vmx(vcpu)->nested.msr_bitmap;

	 
	if (!nested_cpu_has_virt_x2apic_mode(vmcs12))
		return false;

	page = nested_get_page(vcpu, vmcs12->msr_bitmap);
	if (!page) {
		WARN_ON(1);
		return false;
	}
	msr_bitmap_l1 = (unsigned long *)kmap(page);
	if (!msr_bitmap_l1) {
		nested_release_page_clean(page);
		WARN_ON(1);
		return false;
	}

	memset(msr_bitmap_l0, 0xff, PAGE_SIZE);

	if (nested_cpu_has_virt_x2apic_mode(vmcs12)) {
		if (nested_cpu_has_apic_reg_virt(vmcs12))
			for (msr = 0x800; msr <= 0x8ff; msr++)
				nested_vmx_disable_intercept_for_msr(
					msr_bitmap_l1, msr_bitmap_l0,
					msr, MSR_TYPE_R);

		nested_vmx_disable_intercept_for_msr(
				msr_bitmap_l1, msr_bitmap_l0,
				APIC_BASE_MSR + (APIC_TASKPRI >> 4),
				MSR_TYPE_R | MSR_TYPE_W);

		if (nested_cpu_has_vid(vmcs12)) {
			nested_vmx_disable_intercept_for_msr(
				msr_bitmap_l1, msr_bitmap_l0,
				APIC_BASE_MSR + (APIC_EOI >> 4),
				MSR_TYPE_W);
			nested_vmx_disable_intercept_for_msr(
				msr_bitmap_l1, msr_bitmap_l0,
				APIC_BASE_MSR + (APIC_SELF_IPI >> 4),
				MSR_TYPE_W);
		}
	}
	kunmap(page);
	nested_release_page_clean(page);

	return true;
}