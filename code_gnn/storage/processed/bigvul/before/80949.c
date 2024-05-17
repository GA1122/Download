static int handle_ept_misconfig(struct kvm_vcpu *vcpu)
{
	gpa_t gpa;

	 
	gpa = vmcs_read64(GUEST_PHYSICAL_ADDRESS);
	if (!is_guest_mode(vcpu) &&
	    !kvm_io_bus_write(vcpu, KVM_FAST_MMIO_BUS, gpa, 0, NULL)) {
		trace_kvm_fast_mmio(gpa);
		 
		if (!static_cpu_has(X86_FEATURE_HYPERVISOR))
			return kvm_skip_emulated_instruction(vcpu);
		else
			return x86_emulate_instruction(vcpu, gpa, EMULTYPE_SKIP,
						       NULL, 0) == EMULATE_DONE;
	}

	return kvm_mmu_page_fault(vcpu, gpa, PFERR_RSVD_MASK, NULL, 0);
}
