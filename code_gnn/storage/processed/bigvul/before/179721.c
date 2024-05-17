 int tcp_rcv_state_process(struct sock *sk, struct sk_buff *skb,
 			  const struct tcphdr *th, unsigned int len)
 {
 	struct tcp_sock *tp = tcp_sk(sk);
 	struct inet_connection_sock *icsk = inet_csk(sk);
 	int queued = 0;
 	int res;
 
 	tp->rx_opt.saw_tstamp = 0;
 
 	switch (sk->sk_state) {
 	case TCP_CLOSE:
 		goto discard;
 
 	case TCP_LISTEN:
 		if (th->ack)
 			return 1;
 
 		if (th->rst)
  			goto discard;
  
  		if (th->syn) {
// 			if (th->fin)
// 				goto discard;
  			if (icsk->icsk_af_ops->conn_request(sk, skb) < 0)
  				return 1;
  
 			 
 			kfree_skb(skb);
 			return 0;
 		}
 		goto discard;
 
 	case TCP_SYN_SENT:
 		queued = tcp_rcv_synsent_state_process(sk, skb, th, len);
 		if (queued >= 0)
 			return queued;
 
 		 
 		tcp_urg(sk, skb, th);
 		__kfree_skb(skb);
 		tcp_data_snd_check(sk);
 		return 0;
 	}
 
 	res = tcp_validate_incoming(sk, skb, th, 0);
 	if (res <= 0)
 		return -res;
 
 	 
 	if (th->ack) {
 		int acceptable = tcp_ack(sk, skb, FLAG_SLOWPATH) > 0;
 
 		switch (sk->sk_state) {
 		case TCP_SYN_RECV:
 			if (acceptable) {
 				tp->copied_seq = tp->rcv_nxt;
 				smp_mb();
 				tcp_set_state(sk, TCP_ESTABLISHED);
 				sk->sk_state_change(sk);
 
 				 
 				if (sk->sk_socket)
 					sk_wake_async(sk,
 						      SOCK_WAKE_IO, POLL_OUT);
 
 				tp->snd_una = TCP_SKB_CB(skb)->ack_seq;
 				tp->snd_wnd = ntohs(th->window) <<
 					      tp->rx_opt.snd_wscale;
 				tcp_init_wl(tp, TCP_SKB_CB(skb)->seq);
 
 				if (tp->rx_opt.tstamp_ok)
 					tp->advmss -= TCPOLEN_TSTAMP_ALIGNED;
 
 				 
 				icsk->icsk_af_ops->rebuild_header(sk);
 
 				tcp_init_metrics(sk);
 
 				tcp_init_congestion_control(sk);
 
 				 
 				tp->lsndtime = tcp_time_stamp;
 
 				tcp_mtup_init(sk);
 				tcp_initialize_rcv_mss(sk);
 				tcp_init_buffer_space(sk);
 				tcp_fast_path_on(tp);
 			} else {
 				return 1;
 			}
 			break;
 
 		case TCP_FIN_WAIT1:
 			if (tp->snd_una == tp->write_seq) {
 				tcp_set_state(sk, TCP_FIN_WAIT2);
 				sk->sk_shutdown |= SEND_SHUTDOWN;
 				dst_confirm(__sk_dst_get(sk));
 
 				if (!sock_flag(sk, SOCK_DEAD))
 					 
 					sk->sk_state_change(sk);
 				else {
 					int tmo;
 
 					if (tp->linger2 < 0 ||
 					    (TCP_SKB_CB(skb)->end_seq != TCP_SKB_CB(skb)->seq &&
 					     after(TCP_SKB_CB(skb)->end_seq - th->fin, tp->rcv_nxt))) {
 						tcp_done(sk);
 						NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPABORTONDATA);
 						return 1;
 					}
 
 					tmo = tcp_fin_time(sk);
 					if (tmo > TCP_TIMEWAIT_LEN) {
 						inet_csk_reset_keepalive_timer(sk, tmo - TCP_TIMEWAIT_LEN);
 					} else if (th->fin || sock_owned_by_user(sk)) {
 						 
 						inet_csk_reset_keepalive_timer(sk, tmo);
 					} else {
 						tcp_time_wait(sk, TCP_FIN_WAIT2, tmo);
 						goto discard;
 					}
 				}
 			}
 			break;
 
 		case TCP_CLOSING:
 			if (tp->snd_una == tp->write_seq) {
 				tcp_time_wait(sk, TCP_TIME_WAIT, 0);
 				goto discard;
 			}
 			break;
 
 		case TCP_LAST_ACK:
 			if (tp->snd_una == tp->write_seq) {
 				tcp_update_metrics(sk);
 				tcp_done(sk);
 				goto discard;
 			}
 			break;
 		}
 	} else
 		goto discard;
 
 	 
 	tcp_urg(sk, skb, th);
 
 	 
 	switch (sk->sk_state) {
 	case TCP_CLOSE_WAIT:
 	case TCP_CLOSING:
 	case TCP_LAST_ACK:
 		if (!before(TCP_SKB_CB(skb)->seq, tp->rcv_nxt))
 			break;
 	case TCP_FIN_WAIT1:
 	case TCP_FIN_WAIT2:
 		 
 		if (sk->sk_shutdown & RCV_SHUTDOWN) {
 			if (TCP_SKB_CB(skb)->end_seq != TCP_SKB_CB(skb)->seq &&
 			    after(TCP_SKB_CB(skb)->end_seq - th->fin, tp->rcv_nxt)) {
 				NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPABORTONDATA);
 				tcp_reset(sk);
 				return 1;
 			}
 		}
 		 
 	case TCP_ESTABLISHED:
 		tcp_data_queue(sk, skb);
 		queued = 1;
 		break;
 	}
 
 	 
 	if (sk->sk_state != TCP_CLOSE) {
 		tcp_data_snd_check(sk);
 		tcp_ack_snd_check(sk);
 	}
 
 	if (!queued) {
 discard:
 		__kfree_skb(skb);
 	}
 	return 0;
 }