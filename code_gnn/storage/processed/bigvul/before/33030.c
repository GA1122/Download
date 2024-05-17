static int sctp_send_asconf(struct sctp_association *asoc,
			    struct sctp_chunk *chunk)
{
	struct net 	*net = sock_net(asoc->base.sk);
	int		retval = 0;

	 
	if (asoc->addip_last_asconf) {
		list_add_tail(&chunk->list, &asoc->addip_chunk_list);
		goto out;
	}

	 
	sctp_chunk_hold(chunk);
	retval = sctp_primitive_ASCONF(net, asoc, chunk);
	if (retval)
		sctp_chunk_free(chunk);
	else
		asoc->addip_last_asconf = chunk;

out:
	return retval;
}
