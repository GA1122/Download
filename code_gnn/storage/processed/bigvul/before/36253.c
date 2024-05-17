struct sctp_transport *sctp_assoc_lookup_tsn(struct sctp_association *asoc,
					     __u32 tsn)
{
	struct sctp_transport *active;
	struct sctp_transport *match;
	struct sctp_transport *transport;
	struct sctp_chunk *chunk;
	__be32 key = htonl(tsn);

	match = NULL;

	 

	 

	active = asoc->peer.active_path;

	list_for_each_entry(chunk, &active->transmitted,
			transmitted_list) {

		if (key == chunk->subh.data_hdr->tsn) {
			match = active;
			goto out;
		}
	}

	 
	list_for_each_entry(transport, &asoc->peer.transport_addr_list,
			transports) {

		if (transport == active)
			continue;
		list_for_each_entry(chunk, &transport->transmitted,
				transmitted_list) {
			if (key == chunk->subh.data_hdr->tsn) {
				match = transport;
				goto out;
			}
		}
	}
out:
	return match;
}
