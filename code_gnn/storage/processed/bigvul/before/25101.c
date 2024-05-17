static inline bool compare_hash_inputs(const struct rtable *rt1,
				       const struct rtable *rt2)
{
	return ((((__force u32)rt1->rt_key_dst ^ (__force u32)rt2->rt_key_dst) |
		((__force u32)rt1->rt_key_src ^ (__force u32)rt2->rt_key_src) |
		(rt1->rt_iif ^ rt2->rt_iif)) == 0);
}
