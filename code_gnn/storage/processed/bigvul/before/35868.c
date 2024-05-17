struct sctp_chunk *sctp_make_heartbeat_ack(const struct sctp_association *asoc,
				      const struct sctp_chunk *chunk,
				      const void *payload, const size_t paylen)
{
	struct sctp_chunk *retval;

	retval  = sctp_make_control(asoc, SCTP_CID_HEARTBEAT_ACK, 0, paylen);
	if (!retval)
		goto nodata;

	retval->subh.hbs_hdr = sctp_addto_chunk(retval, paylen, payload);

	 
	if (chunk)
		retval->transport = chunk->transport;

nodata:
	return retval;
}
