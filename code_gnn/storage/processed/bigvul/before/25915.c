static int set_segment_reg(struct task_struct *task,
			   unsigned long offset, u16 value)
{
	 
	if (invalid_selector(value))
		return -EIO;

	 
	switch (offset) {
	case offsetof(struct user_regs_struct, cs):
	case offsetof(struct user_regs_struct, ss):
		if (unlikely(value == 0))
			return -EIO;

	default:
		*pt_regs_access(task_pt_regs(task), offset) = value;
		break;

	case offsetof(struct user_regs_struct, gs):
		if (task == current)
			set_user_gs(task_pt_regs(task), value);
		else
			task_user_gs(task) = value;
	}

	return 0;
}
