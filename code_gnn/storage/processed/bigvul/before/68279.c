static int dccp_rcv_closereq(struct sock *sk, struct sk_buff *skb)
{
	int queued = 0;

	 
	if (dccp_sk(sk)->dccps_role != DCCP_ROLE_CLIENT) {
		dccp_send_sync(sk, DCCP_SKB_CB(skb)->dccpd_seq, DCCP_PKT_SYNC);
		return queued;
	}

	 
	switch (sk->sk_state) {
	case DCCP_REQUESTING:
		dccp_send_close(sk, 0);
		dccp_set_state(sk, DCCP_CLOSING);
		break;
	case DCCP_OPEN:
	case DCCP_PARTOPEN:
		 
		queued = 1;
		dccp_fin(sk, skb);
		dccp_set_state(sk, DCCP_PASSIVE_CLOSEREQ);
		 
	case DCCP_PASSIVE_CLOSEREQ:
		sk_wake_async(sk, SOCK_WAKE_WAITD, POLL_HUP);
	}
	return queued;
}
