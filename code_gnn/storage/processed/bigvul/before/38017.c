static inline void __poke_user_per(struct task_struct *child,
				   addr_t addr, addr_t data)
{
	struct per_struct_kernel *dummy = NULL;

	 
	if (addr == (addr_t) &dummy->cr9)
		 
		child->thread.per_user.control =
			data & (PER_EVENT_MASK | PER_CONTROL_MASK);
	else if (addr == (addr_t) &dummy->starting_addr)
		 
		child->thread.per_user.start = data;
	else if (addr == (addr_t) &dummy->ending_addr)
		 
		child->thread.per_user.end = data;
}
