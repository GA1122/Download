static int tcp_is_sackblock_valid(struct tcp_sock *tp, int is_dsack,
				  u32 start_seq, u32 end_seq)
{
	 
	if (after(end_seq, tp->snd_nxt) || !before(start_seq, end_seq))
		return 0;

	 
	if (!before(start_seq, tp->snd_nxt))
		return 0;

	 
	if (after(start_seq, tp->snd_una))
		return 1;

	if (!is_dsack || !tp->undo_marker)
		return 0;

	 
	if (after(end_seq, tp->snd_una))
		return 0;

	if (!before(start_seq, tp->undo_marker))
		return 1;

	 
	if (!after(end_seq, tp->undo_marker))
		return 0;

	 
	return !before(start_seq, end_seq - tp->max_window);
}
