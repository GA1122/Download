static inline unsigned long __peek_user_per(struct task_struct *child,
					    addr_t addr)
{
	struct per_struct_kernel *dummy = NULL;

	if (addr == (addr_t) &dummy->cr9)
		 
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PER_EVENT_IFETCH : child->thread.per_user.control;
	else if (addr == (addr_t) &dummy->cr10)
		 
		return test_thread_flag(TIF_SINGLE_STEP) ?
			0 : child->thread.per_user.start;
	else if (addr == (addr_t) &dummy->cr11)
		 
		return test_thread_flag(TIF_SINGLE_STEP) ?
			PSW_ADDR_INSN : child->thread.per_user.end;
	else if (addr == (addr_t) &dummy->bits)
		 
		return test_thread_flag(TIF_SINGLE_STEP) ?
			(1UL << (BITS_PER_LONG - 1)) : 0;
	else if (addr == (addr_t) &dummy->starting_addr)
		 
		return child->thread.per_user.start;
	else if (addr == (addr_t) &dummy->ending_addr)
		 
		return child->thread.per_user.end;
	else if (addr == (addr_t) &dummy->perc_atmid)
		 
		return (unsigned long)
			child->thread.per_event.cause << (BITS_PER_LONG - 16);
	else if (addr == (addr_t) &dummy->address)
		 
		return child->thread.per_event.address;
	else if (addr == (addr_t) &dummy->access_id)
		 
		return (unsigned long)
			child->thread.per_event.paid << (BITS_PER_LONG - 8);
	return 0;
}
