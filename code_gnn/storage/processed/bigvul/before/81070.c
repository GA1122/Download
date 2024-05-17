static int vmx_set_hv_timer(struct kvm_vcpu *vcpu, u64 guest_deadline_tsc)
{
	struct vcpu_vmx *vmx;
	u64 tscl, guest_tscl, delta_tsc, lapic_timer_advance_cycles;

	if (kvm_mwait_in_guest(vcpu->kvm))
		return -EOPNOTSUPP;

	vmx = to_vmx(vcpu);
	tscl = rdtsc();
	guest_tscl = kvm_read_l1_tsc(vcpu, tscl);
	delta_tsc = max(guest_deadline_tsc, guest_tscl) - guest_tscl;
	lapic_timer_advance_cycles = nsec_to_cycles(vcpu, lapic_timer_advance_ns);

	if (delta_tsc > lapic_timer_advance_cycles)
		delta_tsc -= lapic_timer_advance_cycles;
	else
		delta_tsc = 0;

	 
	if (vcpu->arch.tsc_scaling_ratio != kvm_default_tsc_scaling_ratio &&
			u64_shl_div_u64(delta_tsc,
				kvm_tsc_scaling_ratio_frac_bits,
				vcpu->arch.tsc_scaling_ratio,
				&delta_tsc))
		return -ERANGE;

	 
	if (delta_tsc >> (cpu_preemption_timer_multi + 32))
		return -ERANGE;

	vmx->hv_deadline_tsc = tscl + delta_tsc;
	vmcs_set_bits(PIN_BASED_VM_EXEC_CONTROL,
			PIN_BASED_VMX_PREEMPTION_TIMER);

	return delta_tsc == 0;
}