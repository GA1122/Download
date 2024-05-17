read_block(FILE *fp, pcap_t *p, struct block_cursor *cursor, char *errbuf)
{
	struct pcap_ng_sf *ps;
	int status;
	struct block_header bhdr;
	struct block_trailer *btrlr;
	u_char *bdata;
	size_t data_remaining;

	ps = p->priv;

	status = read_bytes(fp, &bhdr, sizeof(bhdr), 0, errbuf);
	if (status <= 0)
		return (status);	 

	if (p->swapped) {
		bhdr.block_type = SWAPLONG(bhdr.block_type);
		bhdr.total_length = SWAPLONG(bhdr.total_length);
	}

	 
	if (bhdr.total_length < sizeof(struct block_header) +
	    sizeof(struct block_trailer)) {
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "block in pcapng dump file has a length of %u < %" PRIsize,
		    bhdr.total_length,
		    sizeof(struct block_header) + sizeof(struct block_trailer));
		return (-1);
	}

	 
	if ((bhdr.total_length % 4) != 0) {
		 
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "block in pcapng dump file has a length of %u that is not a multiple of 4" PRIsize,
		    bhdr.total_length);
		return (-1);
	}

	 
	if (p->bufsize < bhdr.total_length) {
		 
		void *bigger_buffer;

		if (bhdr.total_length > ps->max_blocksize) {
			pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "pcapng block size %u > maximum %u", bhdr.total_length,
			    ps->max_blocksize);
			return (-1);
		}
		bigger_buffer = realloc(p->buffer, bhdr.total_length);
		if (bigger_buffer == NULL) {
			pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "out of memory");
			return (-1);
		}
		p->buffer = bigger_buffer;
	}

	 
	memcpy(p->buffer, &bhdr, sizeof(bhdr));
	bdata = (u_char *)p->buffer + sizeof(bhdr);
	data_remaining = bhdr.total_length - sizeof(bhdr);
	if (read_bytes(fp, bdata, data_remaining, 1, errbuf) == -1)
		return (-1);

	 
	btrlr = (struct block_trailer *)(bdata + data_remaining - sizeof (struct block_trailer));
	if (p->swapped)
		btrlr->total_length = SWAPLONG(btrlr->total_length);

	 
	if (bhdr.total_length != btrlr->total_length) {
		 
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "block total length in header and trailer don't match");
		return (-1);
	}

	 
	cursor->data = bdata;
	cursor->data_remaining = data_remaining - sizeof(struct block_trailer);
	cursor->block_type = bhdr.block_type;
	return (1);
}
