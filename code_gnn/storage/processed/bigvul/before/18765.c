static struct sock *dccp_v6_hnd_req(struct sock *sk,struct sk_buff *skb)
{
	const struct dccp_hdr *dh = dccp_hdr(skb);
	const struct ipv6hdr *iph = ipv6_hdr(skb);
	struct sock *nsk;
	struct request_sock **prev;
	 
	struct request_sock *req = inet6_csk_search_req(sk, &prev,
							dh->dccph_sport,
							&iph->saddr,
							&iph->daddr,
							inet6_iif(skb));
	if (req != NULL)
		return dccp_check_req(sk, skb, req, prev);

	nsk = __inet6_lookup_established(sock_net(sk), &dccp_hashinfo,
					 &iph->saddr, dh->dccph_sport,
					 &iph->daddr, ntohs(dh->dccph_dport),
					 inet6_iif(skb));
	if (nsk != NULL) {
		if (nsk->sk_state != DCCP_TIME_WAIT) {
			bh_lock_sock(nsk);
			return nsk;
		}
		inet_twsk_put(inet_twsk(nsk));
		return NULL;
	}

	return sk;
}