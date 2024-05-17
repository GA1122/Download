void sctp_assoc_update_retran_path(struct sctp_association *asoc)
{
	struct sctp_transport *trans = asoc->peer.retran_path;
	struct sctp_transport *trans_next = NULL;

	 
	if (asoc->peer.transport_count == 1)
		return;
	 
	if (asoc->peer.active_path == asoc->peer.retran_path &&
	    asoc->peer.active_path->state == SCTP_ACTIVE)
		return;

	 
	for (trans = list_next_entry(trans, transports); 1;
	     trans = list_next_entry(trans, transports)) {
		 
		if (&trans->transports == &asoc->peer.transport_addr_list)
			continue;
		if (trans->state == SCTP_UNCONFIRMED)
			continue;
		trans_next = sctp_trans_elect_best(trans, trans_next);
		 
		if (trans_next->state == SCTP_ACTIVE)
			break;
		 
		if (trans == asoc->peer.retran_path)
			break;
	}

	asoc->peer.retran_path = trans_next;

	pr_debug("%s: association:%p updated new path to addr:%pISpc\n",
		 __func__, asoc, &asoc->peer.retran_path->ipaddr.sa);
}
