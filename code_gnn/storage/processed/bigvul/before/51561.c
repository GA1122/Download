static inline void tcp_init_undo(struct tcp_sock *tp)
{
	tp->undo_marker = tp->snd_una;
	 
	tp->undo_retrans = tp->retrans_out ? : -1;
}