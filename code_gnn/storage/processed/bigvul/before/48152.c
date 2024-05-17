static void vmx_write_tsc_offset(struct kvm_vcpu *vcpu, u64 offset)
{
	if (is_guest_mode(vcpu)) {
		 
		struct vmcs12 *vmcs12;
		 
		vmcs12 = get_vmcs12(vcpu);
		vmcs_write64(TSC_OFFSET, offset +
			(nested_cpu_has(vmcs12, CPU_BASED_USE_TSC_OFFSETING) ?
			 vmcs12->tsc_offset : 0));
	} else {
		trace_kvm_write_tsc_offset(vcpu->vcpu_id,
					   vmcs_read64(TSC_OFFSET), offset);
		vmcs_write64(TSC_OFFSET, offset);
	}
}
