static int nested_vmx_run(struct kvm_vcpu *vcpu, bool launch)
{
	struct vmcs12 *vmcs12;
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u32 interrupt_shadow = vmx_get_interrupt_shadow(vcpu);
	u32 exit_qual;
	int ret;

	if (!nested_vmx_check_permission(vcpu))
		return 1;

	if (!nested_vmx_check_vmcs12(vcpu))
		goto out;

	vmcs12 = get_vmcs12(vcpu);

	if (enable_shadow_vmcs)
		copy_shadow_to_vmcs12(vmx);

	 
	if (interrupt_shadow & KVM_X86_SHADOW_INT_MOV_SS) {
		nested_vmx_failValid(vcpu,
				     VMXERR_ENTRY_EVENTS_BLOCKED_BY_MOV_SS);
		goto out;
	}

	if (vmcs12->launch_state == launch) {
		nested_vmx_failValid(vcpu,
			launch ? VMXERR_VMLAUNCH_NONCLEAR_VMCS
			       : VMXERR_VMRESUME_NONLAUNCHED_VMCS);
		goto out;
	}

	ret = check_vmentry_prereqs(vcpu, vmcs12);
	if (ret) {
		nested_vmx_failValid(vcpu, ret);
		goto out;
	}

	 
	skip_emulated_instruction(vcpu);

	ret = check_vmentry_postreqs(vcpu, vmcs12, &exit_qual);
	if (ret) {
		nested_vmx_entry_failure(vcpu, vmcs12,
					 EXIT_REASON_INVALID_STATE, exit_qual);
		return 1;
	}

	 

	vmx->nested.nested_run_pending = 1;
	ret = enter_vmx_non_root_mode(vcpu);
	if (ret) {
		vmx->nested.nested_run_pending = 0;
		return ret;
	}

	 
	if ((vmcs12->guest_activity_state == GUEST_ACTIVITY_HLT) &&
	    !(vmcs12->vm_entry_intr_info_field & INTR_INFO_VALID_MASK)) {
		vmx->nested.nested_run_pending = 0;
		return kvm_vcpu_halt(vcpu);
	}
	return 1;

out:
	return kvm_skip_emulated_instruction(vcpu);
}
