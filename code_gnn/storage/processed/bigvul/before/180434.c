 static __init int hardware_setup(void)
 {
 	int r = -ENOMEM, i, msr;
 
 	rdmsrl_safe(MSR_EFER, &host_efer);
 
 	for (i = 0; i < ARRAY_SIZE(vmx_msr_index); ++i)
 		kvm_define_shared_msr(i, vmx_msr_index[i]);
 
 	vmx_io_bitmap_a = (unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_io_bitmap_a)
 		return r;
 
 	vmx_io_bitmap_b = (unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_io_bitmap_b)
 		goto out;
 
 	vmx_msr_bitmap_legacy = (unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_msr_bitmap_legacy)
 		goto out1;
 
 	vmx_msr_bitmap_legacy_x2apic =
 				(unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_msr_bitmap_legacy_x2apic)
 		goto out2;
 
 	vmx_msr_bitmap_longmode = (unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_msr_bitmap_longmode)
 		goto out3;
 
 	vmx_msr_bitmap_longmode_x2apic =
 				(unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_msr_bitmap_longmode_x2apic)
 		goto out4;
 
 	if (nested) {
 		vmx_msr_bitmap_nested =
 			(unsigned long *)__get_free_page(GFP_KERNEL);
 		if (!vmx_msr_bitmap_nested)
 			goto out5;
 	}
 
 	vmx_vmread_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_vmread_bitmap)
 		goto out6;
 
 	vmx_vmwrite_bitmap = (unsigned long *)__get_free_page(GFP_KERNEL);
 	if (!vmx_vmwrite_bitmap)
 		goto out7;
 
 	memset(vmx_vmread_bitmap, 0xff, PAGE_SIZE);
 	memset(vmx_vmwrite_bitmap, 0xff, PAGE_SIZE);
 
 	 
 	memset(vmx_io_bitmap_a, 0xff, PAGE_SIZE);
 	clear_bit(0x80, vmx_io_bitmap_a);
 
 	memset(vmx_io_bitmap_b, 0xff, PAGE_SIZE);
 
 	memset(vmx_msr_bitmap_legacy, 0xff, PAGE_SIZE);
 	memset(vmx_msr_bitmap_longmode, 0xff, PAGE_SIZE);
 	if (nested)
 		memset(vmx_msr_bitmap_nested, 0xff, PAGE_SIZE);
 
 	if (setup_vmcs_config(&vmcs_config) < 0) {
 		r = -EIO;
 		goto out8;
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
 
 	memcpy(vmx_msr_bitmap_legacy_x2apic,
 			vmx_msr_bitmap_legacy, PAGE_SIZE);
 	memcpy(vmx_msr_bitmap_longmode_x2apic,
 			vmx_msr_bitmap_longmode, PAGE_SIZE);
  
  	set_bit(0, vmx_vpid_bitmap);  
  
	if (enable_apicv) {
		for (msr = 0x800; msr <= 0x8ff; msr++)
			vmx_disable_intercept_msr_read_x2apic(msr);
		 
		vmx_enable_intercept_msr_read_x2apic(0x802);
		 
		vmx_enable_intercept_msr_read_x2apic(0x839);
		 
		vmx_disable_intercept_msr_write_x2apic(0x808);
		 
		vmx_disable_intercept_msr_write_x2apic(0x80b);
		 
		vmx_disable_intercept_msr_write_x2apic(0x83f);
	}
// 	for (msr = 0x800; msr <= 0x8ff; msr++)
// 		vmx_disable_intercept_msr_read_x2apic(msr);
// 
// 	 
// 	vmx_enable_intercept_msr_read_x2apic(0x802);
// 	 
// 	vmx_enable_intercept_msr_read_x2apic(0x839);
// 	 
// 	vmx_disable_intercept_msr_write_x2apic(0x808);
// 	 
// 	vmx_disable_intercept_msr_write_x2apic(0x80b);
// 	 
// 	vmx_disable_intercept_msr_write_x2apic(0x83f);
  
  	if (enable_ept) {
  		kvm_mmu_set_mask_ptes(0ull,
 			(enable_ept_ad_bits) ? VMX_EPT_ACCESS_BIT : 0ull,
 			(enable_ept_ad_bits) ? VMX_EPT_DIRTY_BIT : 0ull,
 			0ull, VMX_EPT_EXECUTABLE_MASK);
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
 
 	kvm_set_posted_intr_wakeup_handler(wakeup_handler);
 
 	return alloc_kvm_area();
 
 out8:
 	free_page((unsigned long)vmx_vmwrite_bitmap);
 out7:
 	free_page((unsigned long)vmx_vmread_bitmap);
 out6:
 	if (nested)
 		free_page((unsigned long)vmx_msr_bitmap_nested);
 out5:
 	free_page((unsigned long)vmx_msr_bitmap_longmode_x2apic);
 out4:
 	free_page((unsigned long)vmx_msr_bitmap_longmode);
 out3:
 	free_page((unsigned long)vmx_msr_bitmap_legacy_x2apic);
 out2:
 	free_page((unsigned long)vmx_msr_bitmap_legacy);
 out1:
 	free_page((unsigned long)vmx_io_bitmap_b);
 out:
 	free_page((unsigned long)vmx_io_bitmap_a);
 
     return r;
 }