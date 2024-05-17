static void tcp_v4_timewait_ack(struct sock *sk, struct sk_buff *skb)
{
	struct inet_timewait_sock *tw = inet_twsk(sk);
	struct tcp_timewait_sock *tcptw = tcp_twsk(sk);

	tcp_v4_send_ack(skb, tcptw->tw_snd_nxt, tcptw->tw_rcv_nxt,
			tcptw->tw_rcv_wnd >> tw->tw_rcv_wscale,
			tcptw->tw_ts_recent,
			tw->tw_bound_dev_if,
			tcp_twsk_md5_key(tcptw),
			tw->tw_transparent ? IP_REPLY_ARG_NOSRCCHECK : 0
			);

	inet_twsk_put(tw);
}