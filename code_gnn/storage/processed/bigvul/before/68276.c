static void dccp_fin(struct sock *sk, struct sk_buff *skb)
{
	 
	sk->sk_shutdown = SHUTDOWN_MASK;
	sock_set_flag(sk, SOCK_DONE);
	dccp_enqueue_skb(sk, skb);
}
