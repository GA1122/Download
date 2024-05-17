void sctp_assoc_clean_asconf_ack_cache(const struct sctp_association *asoc)
{
	struct sctp_chunk *ack;
	struct sctp_chunk *tmp;

	 
	list_for_each_entry_safe(ack, tmp, &asoc->asconf_ack_list,
				transmitted_list) {
		if (ack->subh.addip_hdr->serial ==
				htonl(asoc->peer.addip_serial))
			break;

		list_del_init(&ack->transmitted_list);
		sctp_chunk_free(ack);
	}
}
