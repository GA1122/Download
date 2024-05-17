void set_personality_ia32(bool x32)
{
	 

	 
	set_thread_flag(TIF_ADDR32);

	 
	if (x32) {
		clear_thread_flag(TIF_IA32);
		set_thread_flag(TIF_X32);
		if (current->mm)
			current->mm->context.ia32_compat = TIF_X32;
		current->personality &= ~READ_IMPLIES_EXEC;
		 
		current_thread_info()->status &= ~TS_COMPAT;
	} else {
		set_thread_flag(TIF_IA32);
		clear_thread_flag(TIF_X32);
		if (current->mm)
			current->mm->context.ia32_compat = TIF_IA32;
		current->personality |= force_personality32;
		 
		current_thread_info()->status |= TS_COMPAT;
	}
}
