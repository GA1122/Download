static void sctp_select_active_and_retran_path(struct sctp_association *asoc)
{
	struct sctp_transport *trans, *trans_pri = NULL, *trans_sec = NULL;
	struct sctp_transport *trans_pf = NULL;

	 
	list_for_each_entry(trans, &asoc->peer.transport_addr_list,
			    transports) {
		 
		if (trans->state == SCTP_INACTIVE ||
		    trans->state == SCTP_UNCONFIRMED)
			continue;
		 
		if (trans->state == SCTP_PF) {
			trans_pf = sctp_trans_elect_best(trans, trans_pf);
			continue;
		}
		 
		if (trans_pri == NULL ||
		    ktime_after(trans->last_time_heard,
				trans_pri->last_time_heard)) {
			trans_sec = trans_pri;
			trans_pri = trans;
		} else if (trans_sec == NULL ||
			   ktime_after(trans->last_time_heard,
				       trans_sec->last_time_heard)) {
			trans_sec = trans;
		}
	}

	 
	if ((asoc->peer.primary_path->state == SCTP_ACTIVE ||
	     asoc->peer.primary_path->state == SCTP_UNKNOWN) &&
	     asoc->peer.primary_path != trans_pri) {
		trans_sec = trans_pri;
		trans_pri = asoc->peer.primary_path;
	}

	 
	if (trans_sec == NULL)
		trans_sec = trans_pri;

	 
	if (trans_pri == NULL) {
		trans_pri = sctp_trans_elect_best(asoc->peer.active_path, trans_pf);
		trans_sec = trans_pri;
	}

	 
	asoc->peer.active_path = trans_pri;
	asoc->peer.retran_path = trans_sec;
}
