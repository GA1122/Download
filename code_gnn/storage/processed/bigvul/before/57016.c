static struct sctp_chunk *sctp_do_ecn_ecne_work(struct sctp_association *asoc,
					   __u32 lowest_tsn,
					   struct sctp_chunk *chunk)
{
	struct sctp_chunk *repl;

	 

	 
	if (TSN_lt(asoc->last_cwr_tsn, lowest_tsn)) {
		struct sctp_transport *transport;

		 
		transport = sctp_assoc_lookup_tsn(asoc, lowest_tsn);

		 
		if (transport)
			sctp_transport_lower_cwnd(transport,
						  SCTP_LOWER_CWND_ECNE);
		asoc->last_cwr_tsn = lowest_tsn;
	}

	 
	repl = sctp_make_cwr(asoc, asoc->last_cwr_tsn, chunk);

	 
	return repl;
}
