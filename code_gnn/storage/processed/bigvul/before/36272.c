static struct sctp_transport *sctp_trans_elect_best(struct sctp_transport *curr,
						    struct sctp_transport *best)
{
	u8 score_curr, score_best;

	if (best == NULL)
		return curr;

	score_curr = sctp_trans_score(curr);
	score_best = sctp_trans_score(best);

	 
	if (score_curr > score_best)
		return curr;
	else if (score_curr == score_best)
		return sctp_trans_elect_tie(curr, best);
	else
		return best;
}
