static void vmx_arm_hv_timer(struct kvm_vcpu *vcpu)
{
	struct vcpu_vmx *vmx = to_vmx(vcpu);
	u64 tscl;
	u32 delta_tsc;

	if (vmx->hv_deadline_tsc == -1)
		return;

	tscl = rdtsc();
	if (vmx->hv_deadline_tsc > tscl)
		 
		delta_tsc = (u32)((vmx->hv_deadline_tsc - tscl) >>
			cpu_preemption_timer_multi);
	else
		delta_tsc = 0;

	vmcs_write32(VMX_PREEMPTION_TIMER_VALUE, delta_tsc);
}