static void check_unshare_flags(unsigned long *flags_ptr)
{
	 
	if (*flags_ptr & CLONE_THREAD)
		*flags_ptr |= CLONE_VM;

	 
	if (*flags_ptr & CLONE_VM)
		*flags_ptr |= CLONE_SIGHAND;

	 
	if (*flags_ptr & CLONE_NEWNS)
		*flags_ptr |= CLONE_FS;
}
