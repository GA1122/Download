static __u32 check_tcp_syn_cookie(__u32 cookie, __be32 saddr, __be32 daddr,
				  __be16 sport, __be16 dport, __u32 sseq,
				  __u32 count, __u32 maxdiff)
{
	__u32 diff;

	 
	cookie -= cookie_hash(saddr, daddr, sport, dport, 0, 0) + sseq;

	 
	diff = (count - (cookie >> COOKIEBITS)) & ((__u32) - 1 >> COOKIEBITS);
	if (diff >= maxdiff)
		return (__u32)-1;

	return (cookie -
		cookie_hash(saddr, daddr, sport, dport, count - diff, 1))
		& COOKIEMASK;	 
}