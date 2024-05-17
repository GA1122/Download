static inline void dccp_v6_send_check(struct sock *sk, struct sk_buff *skb)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct dccp_hdr *dh = dccp_hdr(skb);

	dccp_csum_outgoing(skb);
 	dh->dccph_checksum = dccp_v6_csum_finish(skb, &np->saddr, &np->daddr);
 }