static void vmx_set_virtual_apic_mode(struct kvm_vcpu *vcpu)
{
	u32 sec_exec_control;

	if (!lapic_in_kernel(vcpu))
		return;

	 
	if (is_guest_mode(vcpu)) {
		to_vmx(vcpu)->nested.change_vmcs01_virtual_apic_mode = true;
		return;
	}

	if (!cpu_need_tpr_shadow(vcpu))
		return;

	sec_exec_control = vmcs_read32(SECONDARY_VM_EXEC_CONTROL);
	sec_exec_control &= ~(SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES |
			      SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE);

	switch (kvm_get_apic_mode(vcpu)) {
	case LAPIC_MODE_INVALID:
		WARN_ONCE(true, "Invalid local APIC state");
	case LAPIC_MODE_DISABLED:
		break;
	case LAPIC_MODE_XAPIC:
		if (flexpriority_enabled) {
			sec_exec_control |=
				SECONDARY_EXEC_VIRTUALIZE_APIC_ACCESSES;
			vmx_flush_tlb(vcpu, true);
		}
		break;
	case LAPIC_MODE_X2APIC:
		if (cpu_has_vmx_virtualize_x2apic_mode())
			sec_exec_control |=
				SECONDARY_EXEC_VIRTUALIZE_X2APIC_MODE;
		break;
	}
	vmcs_write32(SECONDARY_VM_EXEC_CONTROL, sec_exec_control);

	vmx_update_msr_bitmap(vcpu);
}