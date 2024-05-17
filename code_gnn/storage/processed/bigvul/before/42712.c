static int nested_vmx_run(struct kvm_vcpu *vcpu, bool launch)
{
	struct vmcs12 *vmcs12;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	int cpu;
	struct loaded_vmcs *vmcs02;
	bool ia32e;
	u32 msr_entry_idx;

	if (!nested_vmx_check_permission(vcpu) ||
	    !nested_vmx_check_vmcs12(vcpu))
		return 1;

	skip_emulated_instruction(vcpu);
	vmcs12 = get_vmcs12(vcpu);

	if (enable_shadow_vmcs)
		copy_shadow_to_vmcs12(vmx);

	 
	if (vmcs12->launch_state == launch) {
		nested_vmx_failValid(vcpu,
			launch ? VMXERR_VMLAUNCH_NONCLEAR_VMCS
			       : VMXERR_VMRESUME_NONLAUNCHED_VMCS);
		return 1;
	}

	if (vmcs12->guest_activity_state != GUEST_ACTIVITY_ACTIVE &&
	    vmcs12->guest_activity_state != GUEST_ACTIVITY_HLT) {
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (!nested_get_vmcs12_pages(vcpu, vmcs12)) {
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (nested_vmx_check_msr_bitmap_controls(vcpu, vmcs12)) {
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (nested_vmx_check_apicv_controls(vcpu, vmcs12)) {
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (nested_vmx_check_msr_switch_controls(vcpu, vmcs12)) {
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (!vmx_control_verify(vmcs12->cpu_based_vm_exec_control,
				vmx->nested.nested_vmx_true_procbased_ctls_low,
				vmx->nested.nested_vmx_procbased_ctls_high) ||
	    !vmx_control_verify(vmcs12->secondary_vm_exec_control,
				vmx->nested.nested_vmx_secondary_ctls_low,
				vmx->nested.nested_vmx_secondary_ctls_high) ||
	    !vmx_control_verify(vmcs12->pin_based_vm_exec_control,
				vmx->nested.nested_vmx_pinbased_ctls_low,
				vmx->nested.nested_vmx_pinbased_ctls_high) ||
	    !vmx_control_verify(vmcs12->vm_exit_controls,
				vmx->nested.nested_vmx_true_exit_ctls_low,
				vmx->nested.nested_vmx_exit_ctls_high) ||
	    !vmx_control_verify(vmcs12->vm_entry_controls,
				vmx->nested.nested_vmx_true_entry_ctls_low,
				vmx->nested.nested_vmx_entry_ctls_high))
	{
		nested_vmx_failValid(vcpu, VMXERR_ENTRY_INVALID_CONTROL_FIELD);
		return 1;
	}

	if (((vmcs12->host_cr0 & VMXON_CR0_ALWAYSON) != VMXON_CR0_ALWAYSON) ||
	    ((vmcs12->host_cr4 & VMXON_CR4_ALWAYSON) != VMXON_CR4_ALWAYSON)) {
		nested_vmx_failValid(vcpu,
			VMXERR_ENTRY_INVALID_HOST_STATE_FIELD);
		return 1;
	}

	if (!nested_cr0_valid(vcpu, vmcs12->guest_cr0) ||
	    ((vmcs12->guest_cr4 & VMXON_CR4_ALWAYSON) != VMXON_CR4_ALWAYSON)) {
		nested_vmx_entry_failure(vcpu, vmcs12,
			EXIT_REASON_INVALID_STATE, ENTRY_FAIL_DEFAULT);
		return 1;
	}
	if (vmcs12->vmcs_link_pointer != -1ull) {
		nested_vmx_entry_failure(vcpu, vmcs12,
			EXIT_REASON_INVALID_STATE, ENTRY_FAIL_VMCS_LINK_PTR);
		return 1;
	}

	 
	if (vmcs12->vm_entry_controls & VM_ENTRY_LOAD_IA32_EFER) {
		ia32e = (vmcs12->vm_entry_controls & VM_ENTRY_IA32E_MODE) != 0;
		if (!kvm_valid_efer(vcpu, vmcs12->guest_ia32_efer) ||
		    ia32e != !!(vmcs12->guest_ia32_efer & EFER_LMA) ||
		    ((vmcs12->guest_cr0 & X86_CR0_PG) &&
		     ia32e != !!(vmcs12->guest_ia32_efer & EFER_LME))) {
			nested_vmx_entry_failure(vcpu, vmcs12,
				EXIT_REASON_INVALID_STATE, ENTRY_FAIL_DEFAULT);
			return 1;
		}
	}

	 
	if (vmcs12->vm_exit_controls & VM_EXIT_LOAD_IA32_EFER) {
		ia32e = (vmcs12->vm_exit_controls &
			 VM_EXIT_HOST_ADDR_SPACE_SIZE) != 0;
		if (!kvm_valid_efer(vcpu, vmcs12->host_ia32_efer) ||
		    ia32e != !!(vmcs12->host_ia32_efer & EFER_LMA) ||
		    ia32e != !!(vmcs12->host_ia32_efer & EFER_LME)) {
			nested_vmx_entry_failure(vcpu, vmcs12,
				EXIT_REASON_INVALID_STATE, ENTRY_FAIL_DEFAULT);
			return 1;
		}
	}

	 

	vmcs02 = nested_get_current_vmcs02(vmx);
	if (!vmcs02)
		return -ENOMEM;

	enter_guest_mode(vcpu);

	vmx->nested.vmcs01_tsc_offset = vmcs_read64(TSC_OFFSET);

	if (!(vmcs12->vm_entry_controls & VM_ENTRY_LOAD_DEBUG_CONTROLS))
		vmx->nested.vmcs01_debugctl = vmcs_read64(GUEST_IA32_DEBUGCTL);

	cpu = get_cpu();
	vmx->loaded_vmcs = vmcs02;
	vmx_vcpu_put(vcpu);
	vmx_vcpu_load(vcpu, cpu);
	vcpu->cpu = cpu;
	put_cpu();

	vmx_segment_cache_clear(vmx);

	prepare_vmcs02(vcpu, vmcs12);

	msr_entry_idx = nested_vmx_load_msr(vcpu,
					    vmcs12->vm_entry_msr_load_addr,
					    vmcs12->vm_entry_msr_load_count);
	if (msr_entry_idx) {
		leave_guest_mode(vcpu);
		vmx_load_vmcs01(vcpu);
		nested_vmx_entry_failure(vcpu, vmcs12,
				EXIT_REASON_MSR_LOAD_FAIL, msr_entry_idx);
		return 1;
	}

	vmcs12->launch_state = 1;

	if (vmcs12->guest_activity_state == GUEST_ACTIVITY_HLT)
		return kvm_vcpu_halt(vcpu);

	vmx->nested.nested_run_pending = 1;

	 
	return 1;
}
