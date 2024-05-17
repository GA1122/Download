void sctp_assoc_rm_peer(struct sctp_association *asoc,
			struct sctp_transport *peer)
{
	struct list_head	*pos;
	struct sctp_transport	*transport;

	pr_debug("%s: association:%p addr:%pISpc\n",
		 __func__, asoc, &peer->ipaddr.sa);

	 
	if (asoc->peer.retran_path == peer)
		sctp_assoc_update_retran_path(asoc);

	 
	list_del_rcu(&peer->transports);

	 
	pos = asoc->peer.transport_addr_list.next;
	transport = list_entry(pos, struct sctp_transport, transports);

	 
	if (asoc->peer.primary_path == peer)
		sctp_assoc_set_primary(asoc, transport);
	if (asoc->peer.active_path == peer)
		asoc->peer.active_path = transport;
	if (asoc->peer.retran_path == peer)
		asoc->peer.retran_path = transport;
	if (asoc->peer.last_data_from == peer)
		asoc->peer.last_data_from = transport;

	 
	if (asoc->init_last_sent_to == peer)
		asoc->init_last_sent_to = NULL;

	 
	if (asoc->shutdown_last_sent_to == peer)
		asoc->shutdown_last_sent_to = NULL;

	 
	if (asoc->addip_last_asconf &&
	    asoc->addip_last_asconf->transport == peer)
		asoc->addip_last_asconf->transport = NULL;

	 
	if (!list_empty(&peer->transmitted)) {
		struct sctp_transport *active = asoc->peer.active_path;
		struct sctp_chunk *ch;

		 
		list_for_each_entry(ch, &peer->transmitted,
					transmitted_list) {
			ch->transport = NULL;
			ch->rtt_in_progress = 0;
		}

		list_splice_tail_init(&peer->transmitted,
					&active->transmitted);

		 
		if (!timer_pending(&active->T3_rtx_timer))
			if (!mod_timer(&active->T3_rtx_timer,
					jiffies + active->rto))
				sctp_transport_hold(active);
	}

	asoc->peer.transport_count--;

	sctp_transport_free(peer);
}
