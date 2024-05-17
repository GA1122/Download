 pcap_ng_check_header(const uint8_t *magic, FILE *fp, u_int precision,
     char *errbuf, int *err)
 {
 	bpf_u_int32 magic_int;
 	size_t amt_read;
 	bpf_u_int32 total_length;
 	bpf_u_int32 byte_order_magic;
 	struct block_header *bhdrp;
 	struct section_header_block *shbp;
 	pcap_t *p;
 	int swapped = 0;
 	struct pcap_ng_sf *ps;
 	int status;
 	struct block_cursor cursor;
 	struct interface_description_block *idbp;
 
 	 
 	*err = 0;
 
 	 
 	memcpy(&magic_int, magic, sizeof(magic_int));
 	if (magic_int != BT_SHB) {
 		 
 		return (NULL);	 
 	}
 
 	 
 	amt_read = fread(&total_length, 1, sizeof(total_length), fp);
 	if (amt_read < sizeof(total_length)) {
 		if (ferror(fp)) {
 			pcap_fmt_errmsg_for_errno(errbuf, PCAP_ERRBUF_SIZE,
 			    errno, "error reading dump file");
 			*err = 1;
 			return (NULL);	 
 		}
 
 		 
 		return (NULL);
 	}
 	amt_read = fread(&byte_order_magic, 1, sizeof(byte_order_magic), fp);
 	if (amt_read < sizeof(byte_order_magic)) {
 		if (ferror(fp)) {
 			pcap_fmt_errmsg_for_errno(errbuf, PCAP_ERRBUF_SIZE,
 			    errno, "error reading dump file");
 			*err = 1;
 			return (NULL);	 
 		}
 
 		 
 		return (NULL);
 	}
 	if (byte_order_magic != BYTE_ORDER_MAGIC) {
 		byte_order_magic = SWAPLONG(byte_order_magic);
 		if (byte_order_magic != BYTE_ORDER_MAGIC) {
 			 
 			return (NULL);
 		}
 		swapped = 1;
 		total_length = SWAPLONG(total_length);
 	}
 
  	 
	if (total_length < sizeof(*bhdrp) + sizeof(*shbp) + sizeof(struct block_trailer)) {
// 	if (total_length < sizeof(*bhdrp) + sizeof(*shbp) + sizeof(struct block_trailer) ||
//             (total_length > BT_SHB_INSANE_MAX)) {
  		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "Section Header Block in pcapng dump file has a length of %u < %" PRIsize,
		    total_length,
		    sizeof(*bhdrp) + sizeof(*shbp) + sizeof(struct block_trailer));
		*err = 1;
		return (NULL);
	}
// 		    "Section Header Block in pcapng dump file has invalid length %" PRIsize " < _%lu_ < %lu (BT_SHB_INSANE_MAX)",
//                               sizeof(*bhdrp) + sizeof(*shbp) + sizeof(struct block_trailer),
//                               total_length,
//                               BT_SHB_INSANE_MAX);
  
	 
	if (total_length > INITIAL_MAX_BLOCKSIZE) {
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
		    "pcapng block size %u > maximum %u",
		    total_length, INITIAL_MAX_BLOCKSIZE);
  		*err = 1;
  		return (NULL);
  	}
  
// 
// 
  	 
 	p = pcap_open_offline_common(errbuf, sizeof (struct pcap_ng_sf));
 	if (p == NULL) {
 		 
 		*err = 1;
 		return (NULL);
 	}
 	p->swapped = swapped;
 	ps = p->priv;
 
 	 
 	switch (precision) {
 
 	case PCAP_TSTAMP_PRECISION_MICRO:
 		ps->user_tsresol = 1000000;
 		break;
 
 	case PCAP_TSTAMP_PRECISION_NANO:
 		ps->user_tsresol = 1000000000;
 		break;
 
 	default:
 		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
 		    "unknown time stamp resolution %u", precision);
 		free(p);
 		*err = 1;
 		return (NULL);
 	}
 
 	p->opt.tstamp_precision = precision;
 
 	 
 	p->bufsize = 2048;
 	if (p->bufsize < total_length)
 		p->bufsize = total_length;
 	p->buffer = malloc(p->bufsize);
 	if (p->buffer == NULL) {
 		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "out of memory");
 		free(p);
 		*err = 1;
 		return (NULL);
 	}
 	ps->max_blocksize = INITIAL_MAX_BLOCKSIZE;
 
 	 
 	bhdrp = (struct block_header *)p->buffer;
 	shbp = (struct section_header_block *)((u_char *)p->buffer + sizeof(struct block_header));
 	bhdrp->block_type = magic_int;
 	bhdrp->total_length = total_length;
 	shbp->byte_order_magic = byte_order_magic;
 	if (read_bytes(fp,
 	    (u_char *)p->buffer + (sizeof(magic_int) + sizeof(total_length) + sizeof(byte_order_magic)),
 	    total_length - (sizeof(magic_int) + sizeof(total_length) + sizeof(byte_order_magic)),
 	    1, errbuf) == -1)
 		goto fail;
 
 	if (p->swapped) {
 		 
 		shbp->major_version = SWAPSHORT(shbp->major_version);
 		shbp->minor_version = SWAPSHORT(shbp->minor_version);
 
 		 
 	}
 	 
 	if (! (shbp->major_version == PCAP_NG_VERSION_MAJOR &&
 	       shbp->minor_version == PCAP_NG_VERSION_MINOR)) {
 		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
 		    "unsupported pcapng savefile version %u.%u",
 		    shbp->major_version, shbp->minor_version);
 		goto fail;
 	}
 	p->version_major = shbp->major_version;
 	p->version_minor = shbp->minor_version;
 
 	 
 	p->opt.tstamp_precision = precision;
 
 	 
 	for (;;) {
 		 
 		status = read_block(fp, p, &cursor, errbuf);
 		if (status == 0) {
 			 
 			pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
 			    "the capture file has no Interface Description Blocks");
 			goto fail;
 		}
 		if (status == -1)
 			goto fail;	 
 		switch (cursor.block_type) {
 
 		case BT_IDB:
 			 
 			idbp = get_from_block_data(&cursor, sizeof(*idbp),
 			    errbuf);
 			if (idbp == NULL)
 				goto fail;	 
 
 			 
 			if (p->swapped) {
 				idbp->linktype = SWAPSHORT(idbp->linktype);
 				idbp->snaplen = SWAPLONG(idbp->snaplen);
 			}
 
 			 
 			if (!add_interface(p, &cursor, errbuf))
 				goto fail;
 
 			goto done;
 
 		case BT_EPB:
 		case BT_SPB:
 		case BT_PB:
 			 
 			pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
 			    "the capture file has a packet block before any Interface Description Blocks");
 			goto fail;
 
 		default:
 			 
 			break;
 		}
 	}
 
 done:
 	p->tzoff = 0;	 
 	p->linktype = linktype_to_dlt(idbp->linktype);
 	p->snapshot = pcap_adjust_snapshot(p->linktype, idbp->snaplen);
 	p->linktype_ext = 0;
 
 	 
 	if (MAX_BLOCKSIZE_FOR_SNAPLEN(max_snaplen_for_dlt(p->linktype)) > ps->max_blocksize)
 		ps->max_blocksize = MAX_BLOCKSIZE_FOR_SNAPLEN(max_snaplen_for_dlt(p->linktype));
 
 	p->next_packet_op = pcap_ng_next_packet;
 	p->cleanup_op = pcap_ng_cleanup;
 
 	return (p);
 
 fail:
 	free(ps->ifaces);
 	free(p->buffer);
 	free(p);
 	*err = 1;
 	return (NULL);
 }