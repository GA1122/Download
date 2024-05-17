static int dccp_rcv_close(struct sock *sk, struct sk_buff *skb)
{
	int queued = 0;

	switch (sk->sk_state) {
	 
	case DCCP_CLOSING:
		 
		if (dccp_sk(sk)->dccps_role != DCCP_ROLE_CLIENT)
			break;
		 
	case DCCP_REQUESTING:
	case DCCP_ACTIVE_CLOSEREQ:
		dccp_send_reset(sk, DCCP_RESET_CODE_CLOSED);
		dccp_done(sk);
		break;
	case DCCP_OPEN:
	case DCCP_PARTOPEN:
		 
		queued = 1;
		dccp_fin(sk, skb);
		dccp_set_state(sk, DCCP_PASSIVE_CLOSE);
		 
	case DCCP_PASSIVE_CLOSE:
		 
		sk_wake_async(sk, SOCK_WAKE_WAITD, POLL_HUP);
	}
	return queued;
}
