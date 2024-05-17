static int dccp_v6_do_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct sk_buff *opt_skb = NULL;

	 

	if (skb->protocol == htons(ETH_P_IP))
		return dccp_v4_do_rcv(sk, skb);

	if (sk_filter(sk, skb))
		goto discard;

	 

	 
	if (np->rxopt.all)
	 
		opt_skb = skb_clone(skb, GFP_ATOMIC);

	if (sk->sk_state == DCCP_OPEN) {  
		if (dccp_rcv_established(sk, skb, dccp_hdr(skb), skb->len))
			goto reset;
		if (opt_skb) {
			 
			__kfree_skb(opt_skb);
		}
		return 0;
	}

	 
	if (sk->sk_state == DCCP_LISTEN) {
		struct sock *nsk = dccp_v6_hnd_req(sk, skb);

		if (nsk == NULL)
			goto discard;
		 
		if (nsk != sk) {
			if (dccp_child_process(sk, nsk, skb))
				goto reset;
			if (opt_skb != NULL)
				__kfree_skb(opt_skb);
			return 0;
		}
	}

	if (dccp_rcv_state_process(sk, skb, dccp_hdr(skb), skb->len))
		goto reset;
	if (opt_skb) {
		 
		__kfree_skb(opt_skb);
	}
	return 0;

reset:
	dccp_v6_ctl_send_reset(sk, skb);
discard:
	if (opt_skb != NULL)
		__kfree_skb(opt_skb);
	kfree_skb(skb);
	return 0;
}
