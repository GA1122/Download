static inline void futex_get_mm(union futex_key *key)
{
	mmgrab(key->private.mm);
	 
	smp_mb__after_atomic();
}
