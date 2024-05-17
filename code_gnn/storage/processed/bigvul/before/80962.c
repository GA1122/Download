static __init int hardware_setup(void)
{
	int r = -ENOMEM, i;

	rdmsrl_safe(MSR_EFER, &host_efer);

	for (i = 0; i < ARRAY_SIZE(vmx_msr_index); ++i)
		kvm_define_shared_msr(i, vmx_msr_index[i]);

	for (i = 0; i < VMX_BITMAP_NR; i++) {
		vmx_bitmap[i] = (unsigned long *)__get_free_page(GFP_KERNEL);
		if (!vmx_bitmap[i])
			goto out;
	}

	memset(vmx_vmread_bitmap, 0xff, PAGE_SIZE);
	memset(vmx_vmwrite_bitmap, 0xff, PAGE_SIZE);

	if (setup_vmcs_config(&vmcs_config) < 0) {
		r = -EIO;
		goto out;
	}

	if (boot_cpu_has(X86_FEATURE_NX))
		kvm_enable_efer_bits(EFER_NX);

	if (!cpu_has_vmx_vpid() || !cpu_has_vmx_invvpid() ||
		!(cpu_has_vmx_invvpid_single() || cpu_has_vmx_invvpid_global()))
		enable_vpid = 0;

	if (!cpu_has_vmx_ept() ||
	    !cpu_has_vmx_ept_4levels() ||
	    !cpu_has_vmx_ept_mt_wb() ||
	    !cpu_has_vmx_invept_global())
		enable_ept = 0;

	if (!cpu_has_vmx_ept_ad_bits() || !enable_ept)
		enable_ept_ad_bits = 0;

	if (!cpu_has_vmx_unrestricted_guest() || !enable_ept)
		enable_unrestricted_guest = 0;

	if (!cpu_has_vmx_flexpriority())
		flexpriority_enabled = 0;

	if (!cpu_has_virtual_nmis())
		enable_vnmi = 0;

	 
	if (!flexpriority_enabled)
		kvm_x86_ops->set_apic_access_page_addr = NULL;

	if (!cpu_has_vmx_tpr_shadow())
		kvm_x86_ops->update_cr8_intercept = NULL;

	if (enable_ept && !cpu_has_vmx_ept_2m_page())
		kvm_disable_largepages();

	if (!cpu_has_vmx_ple()) {
		ple_gap = 0;
		ple_window = 0;
		ple_window_grow = 0;
		ple_window_max = 0;
		ple_window_shrink = 0;
	}

	if (!cpu_has_vmx_apicv()) {
		enable_apicv = 0;
		kvm_x86_ops->sync_pir_to_irr = NULL;
	}

	if (cpu_has_vmx_tsc_scaling()) {
		kvm_has_tsc_control = true;
		kvm_max_tsc_scaling_ratio = KVM_VMX_TSC_MULTIPLIER_MAX;
		kvm_tsc_scaling_ratio_frac_bits = 48;
	}

	set_bit(0, vmx_vpid_bitmap);  

	if (enable_ept)
		vmx_enable_tdp();
	else
		kvm_disable_tdp();

	 
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

	if (!cpu_has_vmx_shadow_vmcs())
		enable_shadow_vmcs = 0;
	if (enable_shadow_vmcs)
		init_vmcs_shadow_fields();

	kvm_set_posted_intr_wakeup_handler(wakeup_handler);
	nested_vmx_setup_ctls_msrs(&vmcs_config.nested, enable_apicv);

	kvm_mce_cap_supported |= MCG_LMCE_P;

	return alloc_kvm_area();

out:
	for (i = 0; i < VMX_BITMAP_NR; i++)
		free_page((unsigned long)vmx_bitmap[i]);

    return r;
}