static int tcp_validate_incoming(struct sock *sk, struct sk_buff *skb,
			      const struct tcphdr *th, int syn_inerr)
{
	const u8 *hash_location;
	struct tcp_sock *tp = tcp_sk(sk);

	 
	if (tcp_fast_parse_options(skb, th, tp, &hash_location) &&
	    tp->rx_opt.saw_tstamp &&
	    tcp_paws_discard(sk, skb)) {
		if (!th->rst) {
			NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_PAWSESTABREJECTED);
			tcp_send_dupack(sk, skb);
			goto discard;
		}
		 
	}

	 
	if (!tcp_sequence(tp, TCP_SKB_CB(skb)->seq, TCP_SKB_CB(skb)->end_seq)) {
		 
		if (!th->rst)
			tcp_send_dupack(sk, skb);
		goto discard;
	}

	 
	if (th->rst) {
		tcp_reset(sk);
		goto discard;
	}

	 
	tcp_replace_ts_recent(tp, TCP_SKB_CB(skb)->seq);

	 

	 
	if (th->syn && !before(TCP_SKB_CB(skb)->seq, tp->rcv_nxt)) {
		if (syn_inerr)
			TCP_INC_STATS_BH(sock_net(sk), TCP_MIB_INERRS);
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPABORTONSYN);
		tcp_reset(sk);
		return -1;
	}

	return 1;

discard:
	__kfree_skb(skb);
	return 0;
}
