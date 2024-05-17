static void tcp_v6_reqsk_send_ack(const struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req)
{
	 
	tcp_v6_send_ack(sk, skb, (sk->sk_state == TCP_LISTEN) ?
			tcp_rsk(req)->snt_isn + 1 : tcp_sk(sk)->snd_nxt,
			tcp_rsk(req)->rcv_nxt, req->rsk_rcv_wnd,
			tcp_time_stamp, req->ts_recent, sk->sk_bound_dev_if,
			tcp_v6_md5_do_lookup(sk, &ipv6_hdr(skb)->daddr),
			0, 0);
}