static void nested_vmx_vmexit(struct kvm_vcpu *vcpu, u32 exit_reason,
			      u32 exit_intr_info,
			      unsigned long exit_qualification)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);

	 
	WARN_ON_ONCE(vmx->nested.nested_run_pending);

	 
	WARN_ON_ONCE(vmx->fail && (vmcs_read32(VM_INSTRUCTION_ERROR) !=
				   VMXERR_ENTRY_INVALID_CONTROL_FIELD));

	leave_guest_mode(vcpu);

	if (vmcs12->cpu_based_vm_exec_control & CPU_BASED_USE_TSC_OFFSETING)
		vcpu->arch.tsc_offset -= vmcs12->tsc_offset;

	if (likely(!vmx->fail)) {
		if (exit_reason == -1)
			sync_vmcs12(vcpu, vmcs12);
		else
			prepare_vmcs12(vcpu, vmcs12, exit_reason, exit_intr_info,
				       exit_qualification);

		if (nested_vmx_store_msr(vcpu, vmcs12->vm_exit_msr_store_addr,
					 vmcs12->vm_exit_msr_store_count))
			nested_vmx_abort(vcpu, VMX_ABORT_SAVE_GUEST_MSR_FAIL);
	}

	vmx_switch_vmcs(vcpu, &vmx->vmcs01);
	vm_entry_controls_reset_shadow(vmx);
	vm_exit_controls_reset_shadow(vmx);
	vmx_segment_cache_clear(vmx);

	 
	vmcs_write32(VM_EXIT_MSR_LOAD_COUNT, vmx->msr_autoload.nr);
	vmcs_write32(VM_ENTRY_MSR_LOAD_COUNT, vmx->msr_autoload.nr);
	vmcs_write64(TSC_OFFSET, vcpu->arch.tsc_offset);
	if (vmx->hv_deadline_tsc == -1)
		vmcs_clear_bits(PIN_BASED_VM_EXEC_CONTROL,
				PIN_BASED_VMX_PREEMPTION_TIMER);
	else
		vmcs_set_bits(PIN_BASED_VM_EXEC_CONTROL,
			      PIN_BASED_VMX_PREEMPTION_TIMER);
	if (kvm_has_tsc_control)
		decache_tsc_multiplier(vmx);

	if (vmx->nested.change_vmcs01_virtual_apic_mode) {
		vmx->nested.change_vmcs01_virtual_apic_mode = false;
		vmx_set_virtual_apic_mode(vcpu);
	} else if (!nested_cpu_has_ept(vmcs12) &&
		   nested_cpu_has2(vmcs12,
				   SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES)) {
		vmx_flush_tlb(vcpu, true);
	}

	 
	vmx->host_rsp = 0;

	 
	if (vmx->nested.apic_access_page) {
		kvm_release_page_dirty(vmx->nested.apic_access_page);
		vmx->nested.apic_access_page = NULL;
	}
	if (vmx->nested.virtual_apic_page) {
		kvm_release_page_dirty(vmx->nested.virtual_apic_page);
		vmx->nested.virtual_apic_page = NULL;
	}
	if (vmx->nested.pi_desc_page) {
		kunmap(vmx->nested.pi_desc_page);
		kvm_release_page_dirty(vmx->nested.pi_desc_page);
		vmx->nested.pi_desc_page = NULL;
		vmx->nested.pi_desc = NULL;
	}

	 
	kvm_make_request(KVM_REQ_APIC_PAGE_RELOAD, vcpu);

	if (enable_shadow_vmcs && exit_reason != -1)
		vmx->nested.sync_shadow_vmcs = true;

	 
	vcpu->arch.mp_state = KVM_MP_STATE_RUNNABLE;

	if (likely(!vmx->fail)) {
		 
		if (nested_exit_intr_ack_set(vcpu) &&
		    exit_reason == EXIT_REASON_EXTERNAL_INTERRUPT &&
		    kvm_cpu_has_interrupt(vcpu)) {
			int irq = kvm_cpu_get_interrupt(vcpu);
			WARN_ON(irq < 0);
			vmcs12->vm_exit_intr_info = irq |
				INTR_INFO_VALID_MASK | INTR_TYPE_EXT_INTR;
		}

		if (exit_reason != -1)
			trace_kvm_nested_vmexit_inject(vmcs12->vm_exit_reason,
						       vmcs12->exit_qualification,
						       vmcs12->idt_vectoring_info_field,
						       vmcs12->vm_exit_intr_info,
						       vmcs12->vm_exit_intr_error_code,
						       KVM_ISA_VMX);

		load_vmcs12_host_state(vcpu, vmcs12);

		return;
	}
	
	 
	nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);

	load_vmcs12_mmu_host_state(vcpu, vmcs12);

	 
	skip_emulated_instruction(vcpu);
	vmx->fail = 0;
}