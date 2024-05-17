static int get_vmx_mem_address(struct kvm_vcpu *vcpu,
				 unsigned long exit_qualification,
				 u32 vmx_instruction_info, gva_t *ret)
{
	 
	int  scaling = vmx_instruction_info & 3;
	int  addr_size = (vmx_instruction_info >> 7) & 7;
	bool is_reg = vmx_instruction_info & (1u << 10);
	int  seg_reg = (vmx_instruction_info >> 15) & 7;
	int  index_reg = (vmx_instruction_info >> 18) & 0xf;
	bool index_is_valid = !(vmx_instruction_info & (1u << 22));
	int  base_reg       = (vmx_instruction_info >> 23) & 0xf;
	bool base_is_valid  = !(vmx_instruction_info & (1u << 27));

	if (is_reg) {
		kvm_queue_exception(vcpu, UD_VECTOR);
		return 1;
	}

	 
	 
	*ret = vmx_get_segment_base(vcpu, seg_reg);
	if (base_is_valid)
		*ret += kvm_register_read(vcpu, base_reg);
	if (index_is_valid)
		*ret += kvm_register_read(vcpu, index_reg)<<scaling;
	*ret += exit_qualification;  

	if (addr_size == 1)  
		*ret &= 0xffffffff;

	 
	return 0;
}
