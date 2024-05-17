static inline unsigned int rt_hash(__be32 daddr, __be32 saddr, int idx,
				   int genid)
{
	return jhash_3words((__force u32)daddr, (__force u32)saddr,
			    idx, genid)
		& rt_hash_mask;
}
