static int ftrace_check_record(struct dyn_ftrace *rec, int enable, int update)
{
	unsigned long flag = 0UL;

	 
	if (enable && (rec->flags & ~FTRACE_FL_MASK))
		flag = FTRACE_FL_ENABLED;

	 
	if (flag &&
	    (!(rec->flags & FTRACE_FL_REGS) != !(rec->flags & FTRACE_FL_REGS_EN)))
		flag |= FTRACE_FL_REGS;

	 
	if ((rec->flags & FTRACE_FL_ENABLED) == flag)
		return FTRACE_UPDATE_IGNORE;

	if (flag) {
		 
		flag ^= rec->flags & FTRACE_FL_ENABLED;

		if (update) {
			rec->flags |= FTRACE_FL_ENABLED;
			if (flag & FTRACE_FL_REGS) {
				if (rec->flags & FTRACE_FL_REGS)
					rec->flags |= FTRACE_FL_REGS_EN;
				else
					rec->flags &= ~FTRACE_FL_REGS_EN;
			}
		}

		 
		if (flag & FTRACE_FL_ENABLED)
			return FTRACE_UPDATE_MAKE_CALL;
		else if (rec->flags & FTRACE_FL_REGS_EN)
			return FTRACE_UPDATE_MODIFY_CALL_REGS;
		else
			return FTRACE_UPDATE_MODIFY_CALL;
	}

	if (update) {
		 
		if (!(rec->flags & ~FTRACE_FL_MASK))
			rec->flags = 0;
		else
			 
			rec->flags &= ~FTRACE_FL_ENABLED;
	}

	return FTRACE_UPDATE_MAKE_NOP;
}
