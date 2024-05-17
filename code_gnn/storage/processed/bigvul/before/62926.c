static int set_invariant_sys_reg(u64 id, void __user *uaddr)
{
	struct sys_reg_params params;
	const struct sys_reg_desc *r;
	int err;
	u64 val = 0;  

	if (!index_to_params(id, &params))
		return -ENOENT;
	r = find_reg(&params, invariant_sys_regs, ARRAY_SIZE(invariant_sys_regs));
	if (!r)
		return -ENOENT;

	err = reg_from_user(&val, uaddr, id);
	if (err)
		return err;

	 
	if (r->val != val)
		return -EINVAL;

	return 0;
}
