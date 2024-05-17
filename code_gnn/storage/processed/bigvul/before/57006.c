static void sctp_cmd_send_asconf(struct sctp_association *asoc)
{
	struct net *net = sock_net(asoc->base.sk);

	 
	if (!list_empty(&asoc->addip_chunk_list)) {
		struct list_head *entry = asoc->addip_chunk_list.next;
		struct sctp_chunk *asconf = list_entry(entry,
						struct sctp_chunk, list);
		list_del_init(entry);

		 
		sctp_chunk_hold(asconf);
		if (sctp_primitive_ASCONF(net, asoc, asconf))
			sctp_chunk_free(asconf);
		else
			asoc->addip_last_asconf = asconf;
	}
}