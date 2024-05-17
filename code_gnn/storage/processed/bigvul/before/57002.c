static void sctp_cmd_process_fwdtsn(struct sctp_ulpq *ulpq,
				    struct sctp_chunk *chunk)
{
	struct sctp_fwdtsn_skip *skip;
	 
	sctp_walk_fwdtsn(skip, chunk) {
		sctp_ulpq_skip(ulpq, ntohs(skip->stream), ntohs(skip->ssn));
	}
}
