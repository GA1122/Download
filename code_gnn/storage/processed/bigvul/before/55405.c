static u8 tcp_sacktag_one(struct sock *sk,
			  struct tcp_sacktag_state *state, u8 sacked,
			  u32 start_seq, u32 end_seq,
			  int dup_sack, int pcount,
			  const struct skb_mstamp *xmit_time)
{
	struct tcp_sock *tp = tcp_sk(sk);
	int fack_count = state->fack_count;

	 
	if (dup_sack && (sacked & TCPCB_RETRANS)) {
		if (tp->undo_marker && tp->undo_retrans > 0 &&
		    after(end_seq, tp->undo_marker))
			tp->undo_retrans--;
		if (sacked & TCPCB_SACKED_ACKED)
			state->reord = min(fack_count, state->reord);
	}

	 
	if (!after(end_seq, tp->snd_una))
		return sacked;

	if (!(sacked & TCPCB_SACKED_ACKED)) {
		tcp_rack_advance(tp, xmit_time, sacked);

		if (sacked & TCPCB_SACKED_RETRANS) {
			 
			if (sacked & TCPCB_LOST) {
				sacked &= ~(TCPCB_LOST|TCPCB_SACKED_RETRANS);
				tp->lost_out -= pcount;
				tp->retrans_out -= pcount;
			}
		} else {
			if (!(sacked & TCPCB_RETRANS)) {
				 
				if (before(start_seq,
					   tcp_highest_sack_seq(tp)))
					state->reord = min(fack_count,
							   state->reord);
				if (!after(end_seq, tp->high_seq))
					state->flag |= FLAG_ORIG_SACK_ACKED;
				if (state->first_sackt.v64 == 0)
					state->first_sackt = *xmit_time;
				state->last_sackt = *xmit_time;
			}

			if (sacked & TCPCB_LOST) {
				sacked &= ~TCPCB_LOST;
				tp->lost_out -= pcount;
			}
		}

		sacked |= TCPCB_SACKED_ACKED;
		state->flag |= FLAG_DATA_SACKED;
		tp->sacked_out += pcount;

		fack_count += pcount;

		 
		if (!tcp_is_fack(tp) && tp->lost_skb_hint &&
		    before(start_seq, TCP_SKB_CB(tp->lost_skb_hint)->seq))
			tp->lost_cnt_hint += pcount;

		if (fack_count > tp->fackets_out)
			tp->fackets_out = fack_count;
	}

	 
	if (dup_sack && (sacked & TCPCB_SACKED_RETRANS)) {
		sacked &= ~TCPCB_SACKED_RETRANS;
		tp->retrans_out -= pcount;
	}

	return sacked;
}