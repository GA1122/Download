static void sctp_wake_up_waiters(struct sock *sk,
				 struct sctp_association *asoc)
{
	struct sctp_association *tmp = asoc;

	 
	if (asoc->ep->sndbuf_policy)
		return __sctp_write_space(asoc);

	 
	if (asoc->base.dead)
		return sctp_write_space(sk);

	 
	for (tmp = list_next_entry(tmp, asocs); 1;
	     tmp = list_next_entry(tmp, asocs)) {
		 
		if (&tmp->asocs == &((sctp_sk(sk))->ep->asocs))
			continue;
		 
		__sctp_write_space(tmp);
		 
		if (tmp == asoc)
			break;
	}
}
