static inline u8 ip4_frag_ecn(u8 tos)
{
	tos = (tos & INET_ECN_MASK) + 1;
	 
	return (tos & 2) ? 0 : tos;
}
