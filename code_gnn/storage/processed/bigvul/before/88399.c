get_from_block_data(struct block_cursor *cursor, size_t chunk_size,
    char *errbuf)
{
	void *data;

	 
	if (cursor->data_remaining < chunk_size) {
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "block of type %u in pcapng dump file is too short",
		    cursor->block_type);
		return (NULL);
	}

	 
	data = cursor->data;
	cursor->data += chunk_size;
	cursor->data_remaining -= chunk_size;
	return (data);
}
