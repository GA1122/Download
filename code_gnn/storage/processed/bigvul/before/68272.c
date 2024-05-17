static int __dccp_rcv_established(struct sock *sk, struct sk_buff *skb,
				  const struct dccp_hdr *dh, const unsigned int len)
{
	struct dccp_sock *dp = dccp_sk(sk);

	switch (dccp_hdr(skb)->dccph_type) {
	case DCCP_PKT_DATAACK:
	case DCCP_PKT_DATA:
		 
		dccp_enqueue_skb(sk, skb);
		return 0;
	case DCCP_PKT_ACK:
		goto discard;
	case DCCP_PKT_RESET:
		 
		dccp_rcv_reset(sk, skb);
		return 0;
	case DCCP_PKT_CLOSEREQ:
		if (dccp_rcv_closereq(sk, skb))
			return 0;
		goto discard;
	case DCCP_PKT_CLOSE:
		if (dccp_rcv_close(sk, skb))
			return 0;
		goto discard;
	case DCCP_PKT_REQUEST:
		 
		if (dp->dccps_role != DCCP_ROLE_LISTEN)
			goto send_sync;
		goto check_seq;
	case DCCP_PKT_RESPONSE:
		if (dp->dccps_role != DCCP_ROLE_CLIENT)
			goto send_sync;
check_seq:
		if (dccp_delta_seqno(dp->dccps_osr,
				     DCCP_SKB_CB(skb)->dccpd_seq) >= 0) {
send_sync:
			dccp_send_sync(sk, DCCP_SKB_CB(skb)->dccpd_seq,
				       DCCP_PKT_SYNC);
		}
		break;
	case DCCP_PKT_SYNC:
		dccp_send_sync(sk, DCCP_SKB_CB(skb)->dccpd_seq,
			       DCCP_PKT_SYNCACK);
		 
		goto discard;
	}

	DCCP_INC_STATS(DCCP_MIB_INERRS);
discard:
	__kfree_skb(skb);
	return 0;
}
