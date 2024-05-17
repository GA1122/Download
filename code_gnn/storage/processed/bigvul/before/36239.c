void sctp_asconf_queue_teardown(struct sctp_association *asoc)
{
	 
	sctp_assoc_free_asconf_acks(asoc);

	 
	sctp_assoc_free_asconf_queue(asoc);

	 
	if (asoc->addip_last_asconf)
		sctp_chunk_free(asoc->addip_last_asconf);
}
