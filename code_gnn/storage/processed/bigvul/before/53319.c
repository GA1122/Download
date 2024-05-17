static void prepare_vmcs02(struct kvm_vcpu *vcpu, struct vmcs12 *vmcs12)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 exec_control;

	vmcs_write16(GUEST_ES_SELECTOR, vmcs12->guest_es_selector);
	vmcs_write16(GUEST_CS_SELECTOR, vmcs12->guest_cs_selector);
	vmcs_write16(GUEST_SS_SELECTOR, vmcs12->guest_ss_selector);
	vmcs_write16(GUEST_DS_SELECTOR, vmcs12->guest_ds_selector);
	vmcs_write16(GUEST_FS_SELECTOR, vmcs12->guest_fs_selector);
	vmcs_write16(GUEST_GS_SELECTOR, vmcs12->guest_gs_selector);
	vmcs_write16(GUEST_LDTR_SELECTOR, vmcs12->guest_ldtr_selector);
	vmcs_write16(GUEST_TR_SELECTOR, vmcs12->guest_tr_selector);
	vmcs_write32(GUEST_ES_LIMIT, vmcs12->guest_es_limit);
	vmcs_write32(GUEST_CS_LIMIT, vmcs12->guest_cs_limit);
	vmcs_write32(GUEST_SS_LIMIT, vmcs12->guest_ss_limit);
	vmcs_write32(GUEST_DS_LIMIT, vmcs12->guest_ds_limit);
	vmcs_write32(GUEST_FS_LIMIT, vmcs12->guest_fs_limit);
	vmcs_write32(GUEST_GS_LIMIT, vmcs12->guest_gs_limit);
	vmcs_write32(GUEST_LDTR_LIMIT, vmcs12->guest_ldtr_limit);
	vmcs_write32(GUEST_TR_LIMIT, vmcs12->guest_tr_limit);
	vmcs_write32(GUEST_GDTR_LIMIT, vmcs12->guest_gdtr_limit);
	vmcs_write32(GUEST_IDTR_LIMIT, vmcs12->guest_idtr_limit);
	vmcs_write32(GUEST_ES_AR_BYTES, vmcs12->guest_es_ar_bytes);
	vmcs_write32(GUEST_CS_AR_BYTES, vmcs12->guest_cs_ar_bytes);
	vmcs_write32(GUEST_SS_AR_BYTES, vmcs12->guest_ss_ar_bytes);
	vmcs_write32(GUEST_DS_AR_BYTES, vmcs12->guest_ds_ar_bytes);
	vmcs_write32(GUEST_FS_AR_BYTES, vmcs12->guest_fs_ar_bytes);
	vmcs_write32(GUEST_GS_AR_BYTES, vmcs12->guest_gs_ar_bytes);
	vmcs_write32(GUEST_LDTR_AR_BYTES, vmcs12->guest_ldtr_ar_bytes);
	vmcs_write32(GUEST_TR_AR_BYTES, vmcs12->guest_tr_ar_bytes);
	vmcs_writel(GUEST_ES_BASE, vmcs12->guest_es_base);
	vmcs_writel(GUEST_CS_BASE, vmcs12->guest_cs_base);
	vmcs_writel(GUEST_SS_BASE, vmcs12->guest_ss_base);
	vmcs_writel(GUEST_DS_BASE, vmcs12->guest_ds_base);
	vmcs_writel(GUEST_FS_BASE, vmcs12->guest_fs_base);
	vmcs_writel(GUEST_GS_BASE, vmcs12->guest_gs_base);
	vmcs_writel(GUEST_LDTR_BASE, vmcs12->guest_ldtr_base);
	vmcs_writel(GUEST_TR_BASE, vmcs12->guest_tr_base);
	vmcs_writel(GUEST_GDTR_BASE, vmcs12->guest_gdtr_base);
	vmcs_writel(GUEST_IDTR_BASE, vmcs12->guest_idtr_base);

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_DEBUG_CONTROLS) {
		kvm_set_dr(vcpu, 7, vmcs12->guest_dr7);
		vmcs_write64(GUEST_IA32_DEBUGCTL, vmcs12->guest_ia32_debugctl);
	} else {
		kvm_set_dr(vcpu, 7, vcpu->arch.dr7);
		vmcs_write64(GUEST_IA32_DEBUGCTL, vmx->nested.vmcs01_debugctl);
	}
	vmcs_write32(VM_ENTRY_INTR_INFO_FIELD,
		vmcs12->vm_entry_intr_info_field);
	vmcs_write32(VM_ENTRY_EXCEPTION_ERROR_CODE,
		vmcs12->vm_entry_exception_error_code);
	vmcs_write32(VM_ENTRY_INSTRUCTION_LEN,
		vmcs12->vm_entry_instruction_len);
	vmcs_write32(GUEST_INTERRUPTIBILITY_INFO,
		vmcs12->guest_interruptibility_info);
	vmcs_write32(GUEST_SYSENTER_CS, vmcs12->guest_sysenter_cs);
	vmx_set_rflags(vcpu, vmcs12->guest_rflags);
	vmcs_writel(GUEST_PENDING_DBG_EXCEPTIONS,
		vmcs12->guest_pending_dbg_exceptions);
	vmcs_writel(GUEST_SYSENTER_ESP, vmcs12->guest_sysenter_esp);
	vmcs_writel(GUEST_SYSENTER_EIP, vmcs12->guest_sysenter_eip);

	if (nested_cpu_has_xsaves(vmcs12))
		vmcs_write64(XSS_EXIT_BITMAP, vmcs12->xss_exit_bitmap);
	vmcs_write64(VMCS_LINK_POINTER, -1ull);

	exec_control = vmcs12->pin_based_vm_exec_control;
	exec_control |= vmcs_config.pin_based_exec_ctrl;
	exec_control &= ~PIN_BASED_VMX_PREEMPTION_TIMER;

	if (nested_cpu_has_posted_intr(vmcs12)) {
		 
		vmx->nested.posted_intr_nv = vmcs12->posted_intr_nv;
		vmx->nested.pi_pending = false;
		vmcs_write16(POSTED_INTR_NV, POSTED_INTR_VECTOR);
		vmcs_write64(POSTED_INTR_DESC_ADDR,
			page_to_phys(vmx->nested.pi_desc_page) +
			(unsigned long)(vmcs12->posted_intr_desc_addr &
			(PAGE_SIZE - 1)));
	} else
		exec_control &= ~PIN_BASED_POSTED_INTR;

	vmcs_write32(PIN_BASED_VM_EXEC_CONTROL, exec_control);

	vmx->nested.preemption_timer_expired = false;
	if (nested_cpu_has_preemption_timer(vmcs12))
		vmx_start_preemption_timer(vcpu);

	 
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MASK,
		enable_ept ? vmcs12->page_fault_error_code_mask : 0);
	vmcs_write32(PAGE_FAULT_ERROR_CODE_MATCH,
		enable_ept ? vmcs12->page_fault_error_code_match : 0);

	if (cpu_has_secondary_exec_ctrls()) {
		exec_control = vmx_secondary_exec_control(vmx);

		 
		exec_control &= ~(SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
				  SECONDARY_EXEC_RDTSCP |
				  SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY |
				  SECONDARY_EXEC_APIC_REGISTER_VIRT |
				  SECONDARY_EXEC_PCOMMIT);
		if (nested_cpu_has(vmcs12,
				CPU_BASED_ACTIVATE_SECONDARY_CONTROLS))
			exec_control |= vmcs12->secondary_vm_exec_control;

		if (exec_control & SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES) {
			 
			if (!vmx->nested.apic_access_page)
				exec_control &=
				  ~SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
			else
				vmcs_write64(APIC_ACCESS_ADDR,
				  page_to_phys(vmx->nested.apic_access_page));
		} else if (!(nested_cpu_has_virt_x2apic_mode(vmcs12)) &&
			    cpu_need_virtualize_apic_accesses(&vmx->vcpu)) {
			exec_control |=
				SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
			kvm_vcpu_reload_apic_access_page(vcpu);
		}

		if (exec_control & SECONDARY_EXEC_VIRTUAL_INTR_DELIVERY) {
			vmcs_write64(EOI_EXIT_BITMAP0,
				vmcs12->eoi_exit_bitmap0);
			vmcs_write64(EOI_EXIT_BITMAP1,
				vmcs12->eoi_exit_bitmap1);
			vmcs_write64(EOI_EXIT_BITMAP2,
				vmcs12->eoi_exit_bitmap2);
			vmcs_write64(EOI_EXIT_BITMAP3,
				vmcs12->eoi_exit_bitmap3);
			vmcs_write16(GUEST_INTR_STATUS,
				vmcs12->guest_intr_status);
		}

		vmcs_write32(SECONDARY_VM_EXEC_CONTROL, exec_control);
	}


	 
	vmx_set_constant_host_state(vmx);

	 
	vmx->host_rsp = 0;

	exec_control = vmx_exec_control(vmx);  
	exec_control &= ~CPU_BASED_VIRTUAL_INTR_PENDING;
	exec_control &= ~CPU_BASED_VIRTUAL_NMI_PENDING;
	exec_control &= ~CPU_BASED_TPR_SHADOW;
	exec_control |= vmcs12->cpu_based_vm_exec_control;

	if (exec_control & CPU_BASED_TPR_SHADOW) {
		vmcs_write64(VIRTUAL_APIC_PAGE_ADDR,
				page_to_phys(vmx->nested.virtual_apic_page));
		vmcs_write32(TPR_THRESHOLD, vmcs12->tpr_threshold);
	}

	if (cpu_has_vmx_msr_bitmap() &&
	    exec_control & CPU_BASED_USE_MSR_BITMAPS) {
		nested_vmx_merge_msr_bitmap(vcpu, vmcs12);
		 
	} else
		exec_control &= ~CPU_BASED_USE_MSR_BITMAPS;

	 
	exec_control &= ~CPU_BASED_USE_IO_BITMAPS;
	exec_control |= CPU_BASED_UNCOND_IO_EXITING;

	vmcs_write32(CPU_BASED_VM_EXEC_CONTROL, exec_control);

	 
	update_exception_bitmap(vcpu);
	vcpu->arch.cr0_guest_owned_bits &= ~vmcs12->cr0_guest_host_mask;
	vmcs_writel(CR0_GUEST_HOST_MASK, ~vcpu->arch.cr0_guest_owned_bits);

	 
	vmcs_write32(VM_EXIT_CONTROLS, vmcs_config.vmexit_ctrl);

	 
	vm_entry_controls_init(vmx, 
		(vmcs12->vm_entry_controls & ~VM_ENTRY_LOAD_IA32_EFER &
			~VM_ENTRY_IA32E_MODE) |
		(vmcs_config.vmentry_ctrl & ~VM_ENTRY_IA32E_MODE));

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_PAT) {
		vmcs_write64(GUEST_IA32_PAT, vmcs12->guest_ia32_pat);
		vcpu->arch.pat = vmcs12->guest_ia32_pat;
	} else if (vmcs_config.vmentry_ctrl & VM_ENTRY_LOAD_IA32_PAT)
		vmcs_write64(GUEST_IA32_PAT, vmx->vcpu.arch.pat);


	set_cr4_guest_host_mask(vmx);

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_BNDCFGS)
		vmcs_write64(GUEST_BNDCFGS, vmcs12->guest_bndcfgs);

	if (vmcs12->cpu_based_vm_exec_control & CPU_BASED_USE_TSC_OFFSETING)
		vmcs_write64(TSC_OFFSET,
			vmx->nested.vmcs01_tsc_offset + vmcs12->tsc_offset);
	else
		vmcs_write64(TSC_OFFSET, vmx->nested.vmcs01_tsc_offset);

	if (enable_vpid) {
		 
		if (nested_cpu_has_vpid(vmcs12) && vmx->nested.vpid02) {
			vmcs_write16(VIRTUAL_PROCESSOR_ID, vmx->nested.vpid02);
			if (vmcs12->virtual_processor_id != vmx->nested.last_vpid) {
				vmx->nested.last_vpid = vmcs12->virtual_processor_id;
				__vmx_flush_tlb(vcpu, to_vmx(vcpu)->nested.vpid02);
			}
		} else {
			vmcs_write16(VIRTUAL_PROCESSOR_ID, vmx->vpid);
			vmx_flush_tlb(vcpu);
		}

	}

	if (nested_cpu_has_ept(vmcs12)) {
		kvm_mmu_unload(vcpu);
		nested_ept_init_mmu_context(vcpu);
	}

	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_EFER)
		vcpu->arch.efer = vmcs12->guest_ia32_efer;
	else if (vmcs12->vm_entry_controls & VM_ENTRY_IA32E_MODE)
		vcpu->arch.efer |= (EFER_LMA | EFER_LME);
	else
		vcpu->arch.efer &= ~(EFER_LMA | EFER_LME);
	 
	vmx_set_efer(vcpu, vcpu->arch.efer);

	 
	vmx_set_cr0(vcpu, vmcs12->guest_cr0);
	vmcs_writel(CR0_READ_SHADOW, nested_read_cr0(vmcs12));

	vmx_set_cr4(vcpu, vmcs12->guest_cr4);
	vmcs_writel(CR4_READ_SHADOW, nested_read_cr4(vmcs12));

	 
	kvm_set_cr3(vcpu, vmcs12->guest_cr3);
	kvm_mmu_reset_context(vcpu);

	if (!enable_ept)
		vcpu->arch.walk_mmu->inject_page_fault = vmx_inject_page_fault_nested;

	 
	if (enable_ept) {
		vmcs_write64(GUEST_PDPTR0, vmcs12->guest_pdptr0);
		vmcs_write64(GUEST_PDPTR1, vmcs12->guest_pdptr1);
		vmcs_write64(GUEST_PDPTR2, vmcs12->guest_pdptr2);
		vmcs_write64(GUEST_PDPTR3, vmcs12->guest_pdptr3);
	}

	kvm_register_write(vcpu, VCPU_REGS_RSP, vmcs12->guest_rsp);
	kvm_register_write(vcpu, VCPU_REGS_RIP, vmcs12->guest_rip);
}
