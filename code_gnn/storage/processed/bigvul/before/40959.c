unsigned long lh_ptr_hash(const void *k)
{
	 
	return (unsigned long)((((ptrdiff_t)k * LH_PRIME) >> 4) & ULONG_MAX);
}
