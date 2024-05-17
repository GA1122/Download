static int walk_sys_regs(struct kvm_vcpu *vcpu, u64 __user *uind)
{
	const struct sys_reg_desc *i1, *i2, *end1, *end2;
	unsigned int total = 0;
	size_t num;

	 
	i1 = get_target_table(vcpu->arch.target, true, &num);
	end1 = i1 + num;
	i2 = sys_reg_descs;
	end2 = sys_reg_descs + ARRAY_SIZE(sys_reg_descs);

	BUG_ON(i1 == end1 || i2 == end2);

	 
	while (i1 || i2) {
		int cmp = cmp_sys_reg(i1, i2);
		 
		if (cmp <= 0) {
			 
			if (i1->reg) {
				if (!copy_reg_to_user(i1, &uind))
					return -EFAULT;
				total++;
			}
		} else {
			 
			if (i2->reg) {
				if (!copy_reg_to_user(i2, &uind))
					return -EFAULT;
				total++;
			}
		}

		if (cmp <= 0 && ++i1 == end1)
			i1 = NULL;
		if (cmp >= 0 && ++i2 == end2)
			i2 = NULL;
	}
	return total;
}
