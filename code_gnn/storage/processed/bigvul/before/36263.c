void sctp_association_free(struct sctp_association *asoc)
{
	struct sock *sk = asoc->base.sk;
	struct sctp_transport *transport;
	struct list_head *pos, *temp;
	int i;

	 
	if (!list_empty(&asoc->asocs)) {
		list_del(&asoc->asocs);

		 
		if (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING))
			sk->sk_ack_backlog--;
	}

	 
	asoc->base.dead = true;

	 
	sctp_outq_free(&asoc->outqueue);

	 
	sctp_ulpq_free(&asoc->ulpq);

	 
	sctp_inq_free(&asoc->base.inqueue);

	sctp_tsnmap_free(&asoc->peer.tsn_map);

	 
	sctp_ssnmap_free(asoc->ssnmap);

	 
	sctp_bind_addr_free(&asoc->base.bind_addr);

	 
	for (i = SCTP_EVENT_TIMEOUT_NONE; i < SCTP_NUM_TIMEOUT_TYPES; ++i) {
		if (del_timer(&asoc->timers[i]))
			sctp_association_put(asoc);
	}

	 
	kfree(asoc->peer.cookie);
	kfree(asoc->peer.peer_random);
	kfree(asoc->peer.peer_chunks);
	kfree(asoc->peer.peer_hmacs);

	 
	list_for_each_safe(pos, temp, &asoc->peer.transport_addr_list) {
		transport = list_entry(pos, struct sctp_transport, transports);
		list_del_rcu(pos);
		sctp_transport_free(transport);
	}

	asoc->peer.transport_count = 0;

	sctp_asconf_queue_teardown(asoc);

	 
	if (asoc->asconf_addr_del_pending != NULL)
		kfree(asoc->asconf_addr_del_pending);

	 
	sctp_auth_destroy_keys(&asoc->endpoint_shared_keys);

	 
	sctp_auth_key_put(asoc->asoc_shared_key);

	sctp_association_put(asoc);
}