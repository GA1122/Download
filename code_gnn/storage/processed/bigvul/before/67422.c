static inline unsigned int fold_hash(unsigned long x, unsigned long y)
{
	 
	return __hash_32(y ^ __hash_32(x));
}
