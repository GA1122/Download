static int handle_vmread(struct kvm_vcpu *vcpu)
{
	unsigned long field;
	u64 field_value;
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	u32 vmx_instruction_info = vmcs_read32(VMX_INSTRUCTION_INFO);
	gva_t gva = 0;

	if (!nested_vmx_check_permission(vcpu) ||
	    !nested_vmx_check_vmcs12(vcpu))
		return 1;

	 
	field = kvm_register_read(vcpu, (((vmx_instruction_info) >> 28) & 0xf));
	 
	if (!vmcs12_read_any(vcpu, field, &field_value)) {
		nested_vmx_failValid(vcpu, VMXERR_UNSUPPORTED_VMCS_COMPONENT);
		skip_emulated_instruction(vcpu);
		return 1;
	}
	 
	if (vmx_instruction_info & (1u << 10)) {
		kvm_register_write(vcpu, (((vmx_instruction_info) >> 3) & 0xf),
			field_value);
	} else {
		if (get_vmx_mem_address(vcpu, exit_qualification,
				vmx_instruction_info, &gva))
			return 1;
		 
		kvm_write_guest_virt_system(&vcpu->arch.emulate_ctxt, gva,
			     &field_value, (is_long_mode(vcpu) ? 8 : 4), NULL);
	}

	nested_vmx_succeed(vcpu);
	skip_emulated_instruction(vcpu);
	return 1;
}