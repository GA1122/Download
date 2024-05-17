static __init int hardware_setup(void)
{
	int r = -ENOMEM, i, msr;

	rdmsrl_safe(MSR_EFER, &host_efer);

	for (i = 0; i < ARRAY_SIZE(vmx_msr_index); ++i)
		kvm_define_shared_msr(i, vmx_msr_index[i]);

	for (i = 0; i < VMX_BITMAP_NR; i++) {
		vmx_bitmap[i] = (unsigned long *)__get_free_page(GFP_KERNEL);
		if (!vmx_bitmap[i])
			goto out;
	}

	vmx_io_bitmap_b = (unsigned long *)__get_free_page(GFP_KERNEL);
	memset(vmx_vmread_bitmap, 0xff, PAGE_SIZE);
	memset(vmx_vmwrite_bitmap, 0xff, PAGE_SIZE);

	 
	memset(vmx_io_bitmap_a, 0xff, PAGE_SIZE);
	clear_bit(0x80, vmx_io_bitmap_a);

	memset(vmx_io_bitmap_b, 0xff, PAGE_SIZE);

	memset(vmx_msr_bitmap_legacy, 0xff, PAGE_SIZE);
	memset(vmx_msr_bitmap_longmode, 0xff, PAGE_SIZE);

	if (setup_vmcs_config(&vmcs_config) < 0) {
		r = -EIO;
		goto out;
	}

	if (boot_cpu_has(X86_FEATURE_NX))
		kvm_enable_efer_bits(EFER_NX);

	if (!cpu_has_vmx_vpid())
		enable_vpid = 0;
	if (!cpu_has_vmx_shadow_vmcs())
		enable_shadow_vmcs = 0;
	if (enable_shadow_vmcs)
		init_vmcs_shadow_fields();

	if (!cpu_has_vmx_ept() ||
	    !cpu_has_vmx_ept_4levels()) {
		enable_ept = 0;
		enable_unrestricted_guest = 0;
		enable_ept_ad_bits = 0;
	}

	if (!cpu_has_vmx_ept_ad_bits())
		enable_ept_ad_bits = 0;

	if (!cpu_has_vmx_unrestricted_guest())
		enable_unrestricted_guest = 0;

	if (!cpu_has_vmx_flexpriority())
		flexpriority_enabled = 0;

	 
	if (!flexpriority_enabled)
		kvm_x86_ops->set_apic_access_page_addr = NULL;

	if (!cpu_has_vmx_tpr_shadow())
		kvm_x86_ops->update_cr8_intercept = NULL;

	if (enable_ept && !cpu_has_vmx_ept_2m_page())
		kvm_disable_largepages();

	if (!cpu_has_vmx_ple())
		ple_gap = 0;

	if (!cpu_has_vmx_apicv())
		enable_apicv = 0;

	if (cpu_has_vmx_tsc_scaling()) {
		kvm_has_tsc_control = true;
		kvm_max_tsc_scaling_ratio = KVM_VMX_TSC_MULTIPLIER_MAX;
		kvm_tsc_scaling_ratio_frac_bits = 48;
	}

	vmx_disable_intercept_for_msr(MSR_FS_BASE, false);
	vmx_disable_intercept_for_msr(MSR_GS_BASE, false);
	vmx_disable_intercept_for_msr(MSR_KERNEL_GS_BASE, true);
	vmx_disable_intercept_for_msr(MSR_IA32_SYSENTER_CS, false);
	vmx_disable_intercept_for_msr(MSR_IA32_SYSENTER_ESP, false);
	vmx_disable_intercept_for_msr(MSR_IA32_SYSENTER_EIP, false);
	vmx_disable_intercept_for_msr(MSR_IA32_BNDCFGS, true);

	memcpy(vmx_msr_bitmap_legacy_x2apic_apicv,
			vmx_msr_bitmap_legacy, PAGE_SIZE);
	memcpy(vmx_msr_bitmap_longmode_x2apic_apicv,
			vmx_msr_bitmap_longmode, PAGE_SIZE);
	memcpy(vmx_msr_bitmap_legacy_x2apic,
			vmx_msr_bitmap_legacy, PAGE_SIZE);
	memcpy(vmx_msr_bitmap_longmode_x2apic,
			vmx_msr_bitmap_longmode, PAGE_SIZE);

	set_bit(0, vmx_vpid_bitmap);  

	for (msr = 0x800; msr <= 0x8ff; msr++) {
		if (msr == 0x839  )
			continue;
		vmx_disable_intercept_msr_x2apic(msr, MSR_TYPE_R, true);
	}

	 
	vmx_disable_intercept_msr_x2apic(0x808, MSR_TYPE_W, true);
	vmx_disable_intercept_msr_x2apic(0x808, MSR_TYPE_R | MSR_TYPE_W, false);

	 
	vmx_disable_intercept_msr_x2apic(0x80b, MSR_TYPE_W, true);
	 
	vmx_disable_intercept_msr_x2apic(0x83f, MSR_TYPE_W, true);

	if (enable_ept) {
		kvm_mmu_set_mask_ptes(VMX_EPT_READABLE_MASK,
			(enable_ept_ad_bits) ? VMX_EPT_ACCESS_BIT : 0ull,
			(enable_ept_ad_bits) ? VMX_EPT_DIRTY_BIT : 0ull,
			0ull, VMX_EPT_EXECUTABLE_MASK,
			cpu_has_vmx_ept_execute_only() ?
				      0ull : VMX_EPT_READABLE_MASK);
		ept_set_mmio_spte_mask();
		kvm_enable_tdp();
	} else
		kvm_disable_tdp();

	update_ple_window_actual_max();

	 
	if (!enable_ept || !enable_ept_ad_bits || !cpu_has_vmx_pml())
		enable_pml = 0;

	if (!enable_pml) {
		kvm_x86_ops->slot_enable_log_dirty = NULL;
		kvm_x86_ops->slot_disable_log_dirty = NULL;
		kvm_x86_ops->flush_log_dirty = NULL;
		kvm_x86_ops->enable_log_dirty_pt_masked = NULL;
	}

	if (cpu_has_vmx_preemption_timer() && enable_preemption_timer) {
		u64 vmx_msr;

		rdmsrl(MSR_IA32_VMX_MISC, vmx_msr);
		cpu_preemption_timer_multi =
			 vmx_msr & VMX_MISC_PREEMPTION_TIMER_RATE_MASK;
	} else {
		kvm_x86_ops->set_hv_timer = NULL;
		kvm_x86_ops->cancel_hv_timer = NULL;
	}

	kvm_set_posted_intr_wakeup_handler(wakeup_handler);

	kvm_mce_cap_supported |= MCG_LMCE_P;

	return alloc_kvm_area();

out:
	for (i = 0; i < VMX_BITMAP_NR; i++)
		free_page((unsigned long)vmx_bitmap[i]);

    return r;
}