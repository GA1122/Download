static unsigned long getreg(struct task_struct *task, unsigned long offset)
{
	switch (offset) {
	case offsetof(struct user_regs_struct, cs):
	case offsetof(struct user_regs_struct, ds):
	case offsetof(struct user_regs_struct, es):
	case offsetof(struct user_regs_struct, fs):
	case offsetof(struct user_regs_struct, gs):
	case offsetof(struct user_regs_struct, ss):
		return get_segment_reg(task, offset);

	case offsetof(struct user_regs_struct, flags):
		return get_flags(task);

#ifdef CONFIG_X86_64
	case offsetof(struct user_regs_struct, fs_base): {
		 
		unsigned int seg = task->thread.fsindex;
		if (task->thread.fs != 0)
			return task->thread.fs;
		if (task == current)
			asm("movl %%fs,%0" : "=r" (seg));
		if (seg != FS_TLS_SEL)
			return 0;
		return get_desc_base(&task->thread.tls_array[FS_TLS]);
	}
	case offsetof(struct user_regs_struct, gs_base): {
		 
		unsigned int seg = task->thread.gsindex;
		if (task->thread.gs != 0)
			return task->thread.gs;
		if (task == current)
			asm("movl %%gs,%0" : "=r" (seg));
		if (seg != GS_TLS_SEL)
			return 0;
		return get_desc_base(&task->thread.tls_array[GS_TLS]);
	}
#endif
	}

	return *pt_regs_access(task_pt_regs(task), offset);
}