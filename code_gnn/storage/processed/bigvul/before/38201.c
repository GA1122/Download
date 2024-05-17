static inline void futex_get_mm(union futex_key *key)
{
	atomic_inc(&key->private.mm->mm_count);
	 
	smp_mb__after_atomic_inc();
}
