void sctp_assoc_control_transport(struct sctp_association *asoc,
				  struct sctp_transport *transport,
				  sctp_transport_cmd_t command,
				  sctp_sn_error_t error)
{
	struct sctp_ulpevent *event;
	struct sockaddr_storage addr;
	int spc_state = 0;
	bool ulp_notify = true;

	 
	switch (command) {
	case SCTP_TRANSPORT_UP:
		 
		if (SCTP_UNCONFIRMED == transport->state &&
		    SCTP_HEARTBEAT_SUCCESS == error)
			spc_state = SCTP_ADDR_CONFIRMED;
		else
			spc_state = SCTP_ADDR_AVAILABLE;
		 
		if (transport->state == SCTP_PF) {
			ulp_notify = false;
			transport->cwnd = asoc->pathmtu;
		}
		transport->state = SCTP_ACTIVE;
		break;

	case SCTP_TRANSPORT_DOWN:
		 
		if (transport->state != SCTP_UNCONFIRMED)
			transport->state = SCTP_INACTIVE;
		else {
			dst_release(transport->dst);
			transport->dst = NULL;
		}

		spc_state = SCTP_ADDR_UNREACHABLE;
		break;

	case SCTP_TRANSPORT_PF:
		transport->state = SCTP_PF;
		ulp_notify = false;
		break;

	default:
		return;
	}

	 
	if (ulp_notify) {
		memset(&addr, 0, sizeof(struct sockaddr_storage));
		memcpy(&addr, &transport->ipaddr,
		       transport->af_specific->sockaddr_len);

		event = sctp_ulpevent_make_peer_addr_change(asoc, &addr,
					0, spc_state, error, GFP_ATOMIC);
		if (event)
			sctp_ulpq_tail_event(&asoc->ulpq, event);
	}

	 
	sctp_select_active_and_retran_path(asoc);
}
