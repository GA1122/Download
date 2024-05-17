static void vmx_complete_atomic_exit(struct vcpu_vmx *vmx)
{
	u32 exit_intr_info = 0;
	u16 basic_exit_reason = (u16)vmx->exit_reason;

	if (!(basic_exit_reason == EXIT_REASON_MCE_DURING_VMENTRY
	      || basic_exit_reason == EXIT_REASON_EXCEPTION_NMI))
		return;

	if (!(vmx->exit_reason & VMX_EXIT_REASONS_FAILED_VMENTRY))
		exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	vmx->exit_intr_info = exit_intr_info;

	 
	if (is_page_fault(exit_intr_info))
		vmx->vcpu.arch.apf.host_apf_reason = kvm_read_and_reset_pf_reason();

	 
	if (basic_exit_reason == EXIT_REASON_MCE_DURING_VMENTRY ||
	    is_machine_check(exit_intr_info))
		kvm_machine_check();

	 
	if (is_nmi(exit_intr_info)) {
		kvm_before_handle_nmi(&vmx->vcpu);
		asm("int $2");
		kvm_after_handle_nmi(&vmx->vcpu);
	}
}
