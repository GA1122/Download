void sctp_assoc_set_primary(struct sctp_association *asoc,
			    struct sctp_transport *transport)
{
	int changeover = 0;

	 
	if (asoc->peer.primary_path != NULL &&
	    asoc->peer.primary_path != transport)
		changeover = 1 ;

	asoc->peer.primary_path = transport;

	 
	memcpy(&asoc->peer.primary_addr, &transport->ipaddr,
	       sizeof(union sctp_addr));

	 
	if ((transport->state == SCTP_ACTIVE) ||
	    (transport->state == SCTP_UNKNOWN))
		asoc->peer.active_path = transport;

	 
	if (!asoc->outqueue.outstanding_bytes && !asoc->outqueue.out_qlen)
		return;

	if (transport->cacc.changeover_active)
		transport->cacc.cycling_changeover = changeover;

	 
	transport->cacc.changeover_active = changeover;

	 
	transport->cacc.next_tsn_at_change = asoc->next_tsn;
}
