static int sctp_cmd_process_init(sctp_cmd_seq_t *commands,
				 struct sctp_association *asoc,
				 struct sctp_chunk *chunk,
				 sctp_init_chunk_t *peer_init,
				 gfp_t gfp)
{
	int error;

	 
	if (!sctp_process_init(asoc, chunk, sctp_source(chunk), peer_init, gfp))
		error = -ENOMEM;
	else
		error = 0;

	return error;
}
