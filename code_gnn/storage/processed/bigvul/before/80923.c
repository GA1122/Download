static int enter_vmx_non_root_mode(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	struct vmcs12 *vmcs12 = get_vmcs12(vcpu);
	u32 msr_entry_idx;
	u32 exit_qual;
	int r;

	enter_guest_mode(vcpu);

	if (!(vmcs12->vm_entry_controls & VM_ENTRY_LOAD_DEBUG_CONTROLS))
		vmx->nested.vmcs01_debugctl = vmcs_read64(GUEST_IA32_DEBUGCTL);

	vmx_switch_vmcs(vcpu, &vmx->nested.vmcs02);
	vmx_segment_cache_clear(vmx);

	if (vmcs12->cpu_based_vm_exec_control & CPU_BASED_USE_TSC_OFFSETING)
		vcpu->arch.tsc_offset += vmcs12->tsc_offset;

	r = EXIT_REASON_INVALID_STATE;
	if (prepare_vmcs02(vcpu, vmcs12, &exit_qual))
		goto fail;

	nested_get_vmcs12_pages(vcpu, vmcs12);

	r = EXIT_REASON_MSR_LOAD_FAIL;
	msr_entry_idx = nested_vmx_load_msr(vcpu,
					    vmcs12->vm_entry_msr_load_addr,
					    vmcs12->vm_entry_msr_load_count);
	if (msr_entry_idx)
		goto fail;

	 
	return 0;

fail:
	if (vmcs12->cpu_based_vm_exec_control & CPU_BASED_USE_TSC_OFFSETING)
		vcpu->arch.tsc_offset -= vmcs12->tsc_offset;
	leave_guest_mode(vcpu);
	vmx_switch_vmcs(vcpu, &vmx->vmcs01);
	nested_vmx_entry_failure(vcpu, vmcs12, r, exit_qual);
	return 1;
}