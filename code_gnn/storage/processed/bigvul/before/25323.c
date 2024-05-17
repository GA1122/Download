static int ptrace_gethbpregs(struct task_struct *tsk, long num,
			     unsigned long  __user *data)
{
	u32 reg;
	int idx, ret = 0;
	struct perf_event *bp;
	struct arch_hw_breakpoint_ctrl arch_ctrl;

	if (num == 0) {
		reg = ptrace_get_hbp_resource_info();
	} else {
		idx = ptrace_hbp_num_to_idx(num);
		if (idx < 0 || idx >= ARM_MAX_HBP_SLOTS) {
			ret = -EINVAL;
			goto out;
		}

		bp = tsk->thread.debug.hbp[idx];
		if (!bp) {
			reg = 0;
			goto put;
		}

		arch_ctrl = counter_arch_bp(bp)->ctrl;

		 
		while (!(arch_ctrl.len & 0x1))
			arch_ctrl.len >>= 1;

		if (num & 0x1)
			reg = bp->attr.bp_addr;
		else
			reg = encode_ctrl_reg(arch_ctrl);
	}

put:
	if (put_user(reg, data))
		ret = -EFAULT;

out:
	return ret;
}