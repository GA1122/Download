static int sctp_sf_check_restart_addrs(const struct sctp_association *new_asoc,
				       const struct sctp_association *asoc,
				       struct sctp_chunk *init,
				       sctp_cmd_seq_t *commands)
{
	struct net *net = sock_net(new_asoc->base.sk);
	struct sctp_transport *new_addr;
	int ret = 1;

	 

	 
	list_for_each_entry(new_addr, &new_asoc->peer.transport_addr_list,
			    transports) {
		if (!list_has_sctp_addr(&asoc->peer.transport_addr_list,
					&new_addr->ipaddr)) {
			sctp_sf_send_restart_abort(net, &new_addr->ipaddr, init,
						   commands);
			ret = 0;
			break;
		}
	}

	 
	return ret;
}
