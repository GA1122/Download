const union sctp_addr *sctp_source(const struct sctp_chunk *chunk)
{
	 
	if (chunk->transport) {
		return &chunk->transport->ipaddr;
	} else {
		 
		return &chunk->source;
	}
}
