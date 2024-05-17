static int tcp_rcv_synsent_state_process(struct sock *sk, struct sk_buff *skb,
					 const struct tcphdr *th, unsigned int len)
{
	const u8 *hash_location;
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct tcp_sock *tp = tcp_sk(sk);
	struct tcp_cookie_values *cvp = tp->cookie_values;
	int saved_clamp = tp->rx_opt.mss_clamp;

	tcp_parse_options(skb, &tp->rx_opt, &hash_location, 0);

	if (th->ack) {
		 
		if (TCP_SKB_CB(skb)->ack_seq != tp->snd_nxt)
			goto reset_and_undo;

		if (tp->rx_opt.saw_tstamp && tp->rx_opt.rcv_tsecr &&
		    !between(tp->rx_opt.rcv_tsecr, tp->retrans_stamp,
			     tcp_time_stamp)) {
			NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_PAWSACTIVEREJECTED);
			goto reset_and_undo;
		}

		 

		if (th->rst) {
			tcp_reset(sk);
			goto discard;
		}

		 
		if (!th->syn)
			goto discard_and_undo;

		 

		TCP_ECN_rcv_synack(tp, th);

		tp->snd_wl1 = TCP_SKB_CB(skb)->seq;
		tcp_ack(sk, skb, FLAG_SLOWPATH);

		 
		tp->rcv_nxt = TCP_SKB_CB(skb)->seq + 1;
		tp->rcv_wup = TCP_SKB_CB(skb)->seq + 1;

		 
		tp->snd_wnd = ntohs(th->window);
		tcp_init_wl(tp, TCP_SKB_CB(skb)->seq);

		if (!tp->rx_opt.wscale_ok) {
			tp->rx_opt.snd_wscale = tp->rx_opt.rcv_wscale = 0;
			tp->window_clamp = min(tp->window_clamp, 65535U);
		}

		if (tp->rx_opt.saw_tstamp) {
			tp->rx_opt.tstamp_ok	   = 1;
			tp->tcp_header_len =
				sizeof(struct tcphdr) + TCPOLEN_TSTAMP_ALIGNED;
			tp->advmss	    -= TCPOLEN_TSTAMP_ALIGNED;
			tcp_store_ts_recent(tp);
		} else {
			tp->tcp_header_len = sizeof(struct tcphdr);
		}

		if (tcp_is_sack(tp) && sysctl_tcp_fack)
			tcp_enable_fack(tp);

		tcp_mtup_init(sk);
		tcp_sync_mss(sk, icsk->icsk_pmtu_cookie);
		tcp_initialize_rcv_mss(sk);

		 
		tp->copied_seq = tp->rcv_nxt;

		if (cvp != NULL &&
		    cvp->cookie_pair_size > 0 &&
		    tp->rx_opt.cookie_plus > 0) {
			int cookie_size = tp->rx_opt.cookie_plus
					- TCPOLEN_COOKIE_BASE;
			int cookie_pair_size = cookie_size
					     + cvp->cookie_desired;

			 
			if (sizeof(cvp->cookie_pair) >= cookie_pair_size) {
				memcpy(&cvp->cookie_pair[cvp->cookie_desired],
				       hash_location, cookie_size);
				cvp->cookie_pair_size = cookie_pair_size;
			}
		}

		smp_mb();
		tcp_set_state(sk, TCP_ESTABLISHED);

		security_inet_conn_established(sk, skb);

		 
		icsk->icsk_af_ops->rebuild_header(sk);

		tcp_init_metrics(sk);

		tcp_init_congestion_control(sk);

		 
		tp->lsndtime = tcp_time_stamp;

		tcp_init_buffer_space(sk);

		if (sock_flag(sk, SOCK_KEEPOPEN))
			inet_csk_reset_keepalive_timer(sk, keepalive_time_when(tp));

		if (!tp->rx_opt.snd_wscale)
			__tcp_fast_path_on(tp, tp->snd_wnd);
		else
			tp->pred_flags = 0;

		if (!sock_flag(sk, SOCK_DEAD)) {
			sk->sk_state_change(sk);
			sk_wake_async(sk, SOCK_WAKE_IO, POLL_OUT);
		}

		if (sk->sk_write_pending ||
		    icsk->icsk_accept_queue.rskq_defer_accept ||
		    icsk->icsk_ack.pingpong) {
			 
			inet_csk_schedule_ack(sk);
			icsk->icsk_ack.lrcvtime = tcp_time_stamp;
			icsk->icsk_ack.ato	 = TCP_ATO_MIN;
			tcp_incr_quickack(sk);
			tcp_enter_quickack_mode(sk);
			inet_csk_reset_xmit_timer(sk, ICSK_TIME_DACK,
						  TCP_DELACK_MAX, TCP_RTO_MAX);

discard:
			__kfree_skb(skb);
			return 0;
		} else {
			tcp_send_ack(sk);
		}
		return -1;
	}

	 

	if (th->rst) {
		 

		goto discard_and_undo;
	}

	 
	if (tp->rx_opt.ts_recent_stamp && tp->rx_opt.saw_tstamp &&
	    tcp_paws_reject(&tp->rx_opt, 0))
		goto discard_and_undo;

	if (th->syn) {
		 
		tcp_set_state(sk, TCP_SYN_RECV);

		if (tp->rx_opt.saw_tstamp) {
			tp->rx_opt.tstamp_ok = 1;
			tcp_store_ts_recent(tp);
			tp->tcp_header_len =
				sizeof(struct tcphdr) + TCPOLEN_TSTAMP_ALIGNED;
		} else {
			tp->tcp_header_len = sizeof(struct tcphdr);
		}

		tp->rcv_nxt = TCP_SKB_CB(skb)->seq + 1;
		tp->rcv_wup = TCP_SKB_CB(skb)->seq + 1;

		 
		tp->snd_wnd    = ntohs(th->window);
		tp->snd_wl1    = TCP_SKB_CB(skb)->seq;
		tp->max_window = tp->snd_wnd;

		TCP_ECN_rcv_syn(tp, th);

		tcp_mtup_init(sk);
		tcp_sync_mss(sk, icsk->icsk_pmtu_cookie);
		tcp_initialize_rcv_mss(sk);

		tcp_send_synack(sk);
#if 0
		 
		return -1;
#else
		goto discard;
#endif
	}
	 

discard_and_undo:
	tcp_clear_options(&tp->rx_opt);
	tp->rx_opt.mss_clamp = saved_clamp;
	goto discard;

reset_and_undo:
	tcp_clear_options(&tp->rx_opt);
	tp->rx_opt.mss_clamp = saved_clamp;
	return 1;
}