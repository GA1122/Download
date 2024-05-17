static int get_vmx_mem_address(struct kvm_vcpu *vcpu,
				 unsigned long exit_qualification,
				 u32 vmx_instruction_info, bool wr, gva_t *ret)
{
	gva_t off;
	bool exn;
	struct kvm_segment s;

	 
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

	 
	 
	off = exit_qualification;  
	if (base_is_valid)
		off += kvm_register_read(vcpu, base_reg);
	if (index_is_valid)
		off += kvm_register_read(vcpu, index_reg)<<scaling;
	vmx_get_segment(vcpu, &s, seg_reg);
	*ret = s.base + off;

	if (addr_size == 1)  
		*ret &= 0xffffffff;

	 
	exn = false;
	if (is_long_mode(vcpu)) {
		 
		exn = is_noncanonical_address(*ret, vcpu);
	} else if (is_protmode(vcpu)) {
		 
		if (wr)
			 
			exn = ((s.type & 0xa) == 0 || (s.type & 8));
		else
			 
			exn = ((s.type & 0xa) == 8);
		if (exn) {
			kvm_queue_exception_e(vcpu, GP_VECTOR, 0);
			return 1;
		}
		 
		exn = (s.unusable != 0);
		 
		exn = exn || (off + sizeof(u64) > s.limit);
	}
	if (exn) {
		kvm_queue_exception_e(vcpu,
				      seg_reg == VCPU_SREG_SS ?
						SS_VECTOR : GP_VECTOR,
				      0);
		return 1;
	}

	return 0;
}
