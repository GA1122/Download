static void tcp_process_loss(struct sock *sk, int flag, bool is_dupack)
{
	struct tcp_sock *tp = tcp_sk(sk);
	bool recovered = !before(tp->snd_una, tp->high_seq);

	if ((flag & FLAG_SND_UNA_ADVANCED) &&
	    tcp_try_undo_loss(sk, false))
		return;

	if (tp->frto) {  
		 
		if ((flag & FLAG_ORIG_SACK_ACKED) &&
		    tcp_try_undo_loss(sk, true))
			return;

		if (after(tp->snd_nxt, tp->high_seq)) {
			if (flag & FLAG_DATA_SACKED || is_dupack)
				tp->frto = 0;  
		} else if (flag & FLAG_SND_UNA_ADVANCED && !recovered) {
			tp->high_seq = tp->snd_nxt;
			__tcp_push_pending_frames(sk, tcp_current_mss(sk),
						  TCP_NAGLE_OFF);
			if (after(tp->snd_nxt, tp->high_seq))
				return;  
			tp->frto = 0;
		}
	}

	if (recovered) {
		 
		tcp_try_undo_recovery(sk);
		return;
	}
	if (tcp_is_reno(tp)) {
		 
		if (after(tp->snd_nxt, tp->high_seq) && is_dupack)
			tcp_add_reno_sack(sk);
		else if (flag & FLAG_SND_UNA_ADVANCED)
			tcp_reset_reno_sack(tp);
	}
	tcp_xmit_retransmit_queue(sk);
}
