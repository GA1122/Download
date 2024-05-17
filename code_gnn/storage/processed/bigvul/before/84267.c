static void get_futex_key_refs(union futex_key *key)
{
	if (!key->both.ptr)
		return;

	 
	if (!IS_ENABLED(CONFIG_MMU)) {
		smp_mb();  
		return;
	}

	switch (key->both.offset & (FUT_OFF_INODE|FUT_OFF_MMSHARED)) {
	case FUT_OFF_INODE:
		ihold(key->shared.inode);  
		break;
	case FUT_OFF_MMSHARED:
		futex_get_mm(key);  
		break;
	default:
		 
		smp_mb();  
	}
}
