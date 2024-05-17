static void udp_sk_rx_dst_set(struct sock *sk, struct dst_entry *dst)
{
	struct dst_entry *old;

	dst_hold(dst);
	old = xchg(&sk->sk_rx_dst, dst);
	dst_release(old);
}