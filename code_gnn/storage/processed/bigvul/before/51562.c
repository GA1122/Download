static bool tcp_is_sackblock_valid(struct tcp_sock *tp, bool is_dsack,
				   u32 start_seq, u32 end_seq)
{
	 
	if (after(end_seq, tp->snd_nxt) || !before(start_seq, end_seq))
		return false;

	 
	if (!before(start_seq, tp->snd_nxt))
		return false;

	 
	if (after(start_seq, tp->snd_una))
		return true;

	if (!is_dsack || !tp->undo_marker)
		return false;

	 
	if (after(end_seq, tp->snd_una))
		return false;

	if (!before(start_seq, tp->undo_marker))
		return true;

	 
	if (!after(end_seq, tp->undo_marker))
		return false;

	 
	return !before(start_seq, end_seq - tp->max_window);
}
