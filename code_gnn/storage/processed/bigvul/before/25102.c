static inline int compare_keys(struct rtable *rt1, struct rtable *rt2)
{
	return (((__force u32)rt1->rt_key_dst ^ (__force u32)rt2->rt_key_dst) |
		((__force u32)rt1->rt_key_src ^ (__force u32)rt2->rt_key_src) |
		(rt1->rt_mark ^ rt2->rt_mark) |
		(rt1->rt_key_tos ^ rt2->rt_key_tos) |
		(rt1->rt_oif ^ rt2->rt_oif) |
		(rt1->rt_iif ^ rt2->rt_iif)) == 0;
}
