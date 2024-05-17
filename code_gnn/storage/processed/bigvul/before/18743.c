int dccp_v4_do_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct dccp_hdr *dh = dccp_hdr(skb);

	if (sk->sk_state == DCCP_OPEN) {  
		if (dccp_rcv_established(sk, skb, dh, skb->len))
			goto reset;
		return 0;
	}

	 
	if (sk->sk_state == DCCP_LISTEN) {
		struct sock *nsk = dccp_v4_hnd_req(sk, skb);

		if (nsk == NULL)
			goto discard;

		if (nsk != sk) {
			if (dccp_child_process(sk, nsk, skb))
				goto reset;
			return 0;
		}
	}

	if (dccp_rcv_state_process(sk, skb, dh, skb->len))
		goto reset;
	return 0;

reset:
	dccp_v4_ctl_send_reset(sk, skb);
discard:
	kfree_skb(skb);
	return 0;
}
