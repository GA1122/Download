sctp_assoc_choose_alter_transport(struct sctp_association *asoc,
				  struct sctp_transport *last_sent_to)
{
	 
	if (last_sent_to == NULL) {
		return asoc->peer.active_path;
	} else {
		if (last_sent_to == asoc->peer.retran_path)
			sctp_assoc_update_retran_path(asoc);

		return asoc->peer.retran_path;
	}
}
