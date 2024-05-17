static void unix_dgram_disconnected(struct sock *sk, struct sock *other)
{
	if (!skb_queue_empty(&sk->sk_receive_queue)) {
		skb_queue_purge(&sk->sk_receive_queue);
		wake_up_interruptible_all(&unix_sk(sk)->peer_wait);

		 
		if (!sock_flag(other, SOCK_DEAD) && unix_peer(other) == sk) {
			other->sk_err = ECONNRESET;
			other->sk_error_report(other);
		}
	}
}
