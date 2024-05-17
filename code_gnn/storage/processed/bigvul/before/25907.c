ptrace_modify_breakpoint(struct perf_event *bp, int len, int type,
			 struct task_struct *tsk, int disabled)
{
	int err;
	int gen_len, gen_type;
	struct perf_event_attr attr;

	 
	if (!bp)
		return -EINVAL;

	err = arch_bp_generic_fields(len, type, &gen_len, &gen_type);
	if (err)
		return err;

	attr = bp->attr;
	attr.bp_len = gen_len;
	attr.bp_type = gen_type;
	attr.disabled = disabled;

	return modify_user_hw_breakpoint(bp, &attr);
}
