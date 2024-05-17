static inline void __poke_user_per_compat(struct task_struct *child,
					  addr_t addr, __u32 data)
{
	struct compat_per_struct_kernel *dummy32 = NULL;

	if (addr == (addr_t) &dummy32->cr9)
		 
		child->thread.per_user.control =
			data & (PER_EVENT_MASK | PER_CONTROL_MASK);
	else if (addr == (addr_t) &dummy32->starting_addr)
		 
		child->thread.per_user.start = data;
	else if (addr == (addr_t) &dummy32->ending_addr)
		 
		child->thread.per_user.end = data;
}
