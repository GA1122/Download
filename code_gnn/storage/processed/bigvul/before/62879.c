static int emulate_cp(struct kvm_vcpu *vcpu,
		      struct sys_reg_params *params,
		      const struct sys_reg_desc *table,
		      size_t num)
{
	const struct sys_reg_desc *r;

	if (!table)
		return -1;	 

	r = find_reg(params, table, num);

	if (r) {
		 
		BUG_ON(!r->access);

		if (likely(r->access(vcpu, params, r))) {
			 
			kvm_skip_instr(vcpu, kvm_vcpu_trap_il_is32bit(vcpu));
			 
			return 0;
		}
	}

	 
	return -1;
}
