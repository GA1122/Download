static int tcp_ack(struct sock *sk, const struct sk_buff *skb, int flag)
{
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_sacktag_state sack_state;
	u32 prior_snd_una = tp->snd_una;
	u32 ack_seq = TCP_SKB_CB(skb)->seq;
	u32 ack = TCP_SKB_CB(skb)->ack_seq;
	bool is_dupack = false;
	u32 prior_fackets;
	int prior_packets = tp->packets_out;
	const int prior_unsacked = tp->packets_out - tp->sacked_out;
	int acked = 0;  

	sack_state.first_sackt.v64 = 0;

	 
	prefetchw(sk->sk_write_queue.next);

	 
	if (before(ack, prior_snd_una)) {
		 
		if (before(ack, prior_snd_una - tp->max_window)) {
			tcp_send_challenge_ack(sk, skb);
			return -1;
		}
		goto old_ack;
	}

	 
	if (after(ack, tp->snd_nxt))
		goto invalid_ack;

	if (icsk->icsk_pending == ICSK_TIME_EARLY_RETRANS ||
	    icsk->icsk_pending == ICSK_TIME_LOSS_PROBE)
		tcp_rearm_rto(sk);

	if (after(ack, prior_snd_una)) {
		flag |= FLAG_SND_UNA_ADVANCED;
		icsk->icsk_retransmits = 0;
	}

	prior_fackets = tp->fackets_out;

	 
	if (flag & FLAG_UPDATE_TS_RECENT)
		tcp_replace_ts_recent(tp, TCP_SKB_CB(skb)->seq);

	if (!(flag & FLAG_SLOWPATH) && after(ack, prior_snd_una)) {
		 
		tcp_update_wl(tp, ack_seq);
		tcp_snd_una_update(tp, ack);
		flag |= FLAG_WIN_UPDATE;

		tcp_in_ack_event(sk, CA_ACK_WIN_UPDATE);

		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPHPACKS);
	} else {
		u32 ack_ev_flags = CA_ACK_SLOWPATH;

		if (ack_seq != TCP_SKB_CB(skb)->end_seq)
			flag |= FLAG_DATA;
		else
			NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPPUREACKS);

		flag |= tcp_ack_update_window(sk, skb, ack, ack_seq);

		if (TCP_SKB_CB(skb)->sacked)
			flag |= tcp_sacktag_write_queue(sk, skb, prior_snd_una,
							&sack_state);

		if (tcp_ecn_rcv_ecn_echo(tp, tcp_hdr(skb))) {
			flag |= FLAG_ECE;
			ack_ev_flags |= CA_ACK_ECE;
		}

		if (flag & FLAG_WIN_UPDATE)
			ack_ev_flags |= CA_ACK_WIN_UPDATE;

		tcp_in_ack_event(sk, ack_ev_flags);
	}

	 
	sk->sk_err_soft = 0;
	icsk->icsk_probes_out = 0;
	tp->rcv_tstamp = tcp_time_stamp;
	if (!prior_packets)
		goto no_queue;

	 
	acked = tp->packets_out;
	flag |= tcp_clean_rtx_queue(sk, prior_fackets, prior_snd_una,
				    &sack_state);
	acked -= tp->packets_out;

	if (tcp_ack_is_dubious(sk, flag)) {
		is_dupack = !(flag & (FLAG_SND_UNA_ADVANCED | FLAG_NOT_DUP));
		tcp_fastretrans_alert(sk, acked, prior_unsacked,
				      is_dupack, flag);
	}
	if (tp->tlp_high_seq)
		tcp_process_tlp_ack(sk, ack, flag);

	 
	if (tcp_may_raise_cwnd(sk, flag))
		tcp_cong_avoid(sk, ack, acked);

	if ((flag & FLAG_FORWARD_PROGRESS) || !(flag & FLAG_NOT_DUP)) {
		struct dst_entry *dst = __sk_dst_get(sk);
		if (dst)
			dst_confirm(dst);
	}

	if (icsk->icsk_pending == ICSK_TIME_RETRANS)
		tcp_schedule_loss_probe(sk);
	tcp_update_pacing_rate(sk);
	return 1;

no_queue:
	 
	if (flag & FLAG_DSACKING_ACK)
		tcp_fastretrans_alert(sk, acked, prior_unsacked,
				      is_dupack, flag);
	 
	if (tcp_send_head(sk))
		tcp_ack_probe(sk);

	if (tp->tlp_high_seq)
		tcp_process_tlp_ack(sk, ack, flag);
	return 1;

invalid_ack:
	SOCK_DEBUG(sk, "Ack %u after %u:%u\n", ack, tp->snd_una, tp->snd_nxt);
	return -1;

old_ack:
	 
	if (TCP_SKB_CB(skb)->sacked) {
		flag |= tcp_sacktag_write_queue(sk, skb, prior_snd_una,
						&sack_state);
		tcp_fastretrans_alert(sk, acked, prior_unsacked,
				      is_dupack, flag);
	}

	SOCK_DEBUG(sk, "Ack %u before %u:%u\n", ack, tp->snd_una, tp->snd_nxt);
	return 0;
}