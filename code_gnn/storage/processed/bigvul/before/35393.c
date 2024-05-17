void set_personality_64bit(void)
{
	 

	 
	clear_thread_flag(TIF_IA32);
	clear_thread_flag(TIF_ADDR32);
	clear_thread_flag(TIF_X32);

	 
	if (current->mm)
		current->mm->context.ia32_compat = 0;

	 
	current->personality &= ~READ_IMPLIES_EXEC;
}
