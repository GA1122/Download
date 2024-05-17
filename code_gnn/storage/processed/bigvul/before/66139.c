static inline void syn_ack_recalc(struct request_sock *req, const int thresh,
				  const int max_retries,
				  const u8 rskq_defer_accept,
				  int *expire, int *resend)
{
	if (!rskq_defer_accept) {
		*expire = req->num_timeout >= thresh;
		*resend = 1;
		return;
	}
	*expire = req->num_timeout >= thresh &&
		  (!inet_rsk(req)->acked || req->num_timeout >= max_retries);
	 
	*resend = !inet_rsk(req)->acked ||
		  req->num_timeout >= rskq_defer_accept - 1;
}
