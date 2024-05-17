pcap_ng_next_packet(pcap_t *p, struct pcap_pkthdr *hdr, u_char **data)
{
	struct pcap_ng_sf *ps = p->priv;
	struct block_cursor cursor;
	int status;
	struct enhanced_packet_block *epbp;
	struct simple_packet_block *spbp;
	struct packet_block *pbp;
	bpf_u_int32 interface_id = 0xFFFFFFFF;
	struct interface_description_block *idbp;
	struct section_header_block *shbp;
	FILE *fp = p->rfile;
	uint64_t t, sec, frac;

	 
	for (;;) {
		 
		status = read_block(fp, p, &cursor, p->errbuf);
		if (status == 0)
			return (1);	 
		if (status == -1)
			return (-1);	 
		switch (cursor.block_type) {

		case BT_EPB:
			 
			epbp = get_from_block_data(&cursor, sizeof(*epbp),
			    p->errbuf);
			if (epbp == NULL)
				return (-1);	 

			 
			if (p->swapped) {
				 
				interface_id = SWAPLONG(epbp->interface_id);
				hdr->caplen = SWAPLONG(epbp->caplen);
				hdr->len = SWAPLONG(epbp->len);
				t = ((uint64_t)SWAPLONG(epbp->timestamp_high)) << 32 |
				    SWAPLONG(epbp->timestamp_low);
			} else {
				interface_id = epbp->interface_id;
				hdr->caplen = epbp->caplen;
				hdr->len = epbp->len;
				t = ((uint64_t)epbp->timestamp_high) << 32 |
				    epbp->timestamp_low;
			}
			goto found;

		case BT_SPB:
			 
			spbp = get_from_block_data(&cursor, sizeof(*spbp),
			    p->errbuf);
			if (spbp == NULL)
				return (-1);	 

			 
			interface_id = 0;

			 
			if (p->swapped) {
				 
				hdr->len = SWAPLONG(spbp->len);
			} else
				hdr->len = spbp->len;

			 
			hdr->caplen = hdr->len;
			if (hdr->caplen > (bpf_u_int32)p->snapshot)
				hdr->caplen = p->snapshot;
			t = 0;	 
			goto found;

		case BT_PB:
			 
			pbp = get_from_block_data(&cursor, sizeof(*pbp),
			    p->errbuf);
			if (pbp == NULL)
				return (-1);	 

			 
			if (p->swapped) {
				 
				interface_id = SWAPSHORT(pbp->interface_id);
				hdr->caplen = SWAPLONG(pbp->caplen);
				hdr->len = SWAPLONG(pbp->len);
				t = ((uint64_t)SWAPLONG(pbp->timestamp_high)) << 32 |
				    SWAPLONG(pbp->timestamp_low);
			} else {
				interface_id = pbp->interface_id;
				hdr->caplen = pbp->caplen;
				hdr->len = pbp->len;
				t = ((uint64_t)pbp->timestamp_high) << 32 |
				    pbp->timestamp_low;
			}
			goto found;

		case BT_IDB:
			 
			idbp = get_from_block_data(&cursor, sizeof(*idbp),
			    p->errbuf);
			if (idbp == NULL)
				return (-1);	 

			 
			if (p->swapped) {
				idbp->linktype = SWAPSHORT(idbp->linktype);
				idbp->snaplen = SWAPLONG(idbp->snaplen);
			}

			 
			if (p->linktype != idbp->linktype) {
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "an interface has a type %u different from the type of the first interface",
				    idbp->linktype);
				return (-1);
			}

			 
			if ((bpf_u_int32)p->snapshot !=
			    pcap_adjust_snapshot(p->linktype, idbp->snaplen)) {
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "an interface has a snapshot length %u different from the type of the first interface",
				    idbp->snaplen);
				return (-1);
			}

			 
			if (!add_interface(p, &cursor, p->errbuf))
				return (-1);
			break;

		case BT_SHB:
			 
			shbp = get_from_block_data(&cursor, sizeof(*shbp),
			    p->errbuf);
			if (shbp == NULL)
				return (-1);	 

			 
			if (p->swapped) {
				shbp->byte_order_magic =
				    SWAPLONG(shbp->byte_order_magic);
				shbp->major_version =
				    SWAPSHORT(shbp->major_version);
			}

			 
			switch (shbp->byte_order_magic) {

			case BYTE_ORDER_MAGIC:
				 
				break;

			case SWAPLONG(BYTE_ORDER_MAGIC):
				 
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "the file has sections with different byte orders");
				return (-1);

			default:
				 
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "the file has a section with a bad byte order magic field");
				return (-1);
			}

			 
			if (shbp->major_version != PCAP_NG_VERSION_MAJOR) {
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "unknown pcapng savefile major version number %u",
				    shbp->major_version);
				return (-1);
			}

			 
			ps->ifcount = 0;
			break;

		default:
			 
			break;
		}
	}

found:
	 
	if (interface_id >= ps->ifcount) {
		 
		pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
		    "a packet arrived on interface %u, but there's no Interface Description Block for that interface",
		    interface_id);
		return (-1);
	}

	if (hdr->caplen > (bpf_u_int32)p->snapshot) {
		pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
		    "invalid packet capture length %u, bigger than "
		    "snaplen of %d", hdr->caplen, p->snapshot);
		return (-1);
	}

	 
	sec = t / ps->ifaces[interface_id].tsresol + ps->ifaces[interface_id].tsoffset;
	frac = t % ps->ifaces[interface_id].tsresol;

	 
	switch (ps->ifaces[interface_id].scale_type) {

	case PASS_THROUGH:
		 
		break;

	case SCALE_UP_DEC:
		 
		frac *= ps->ifaces[interface_id].scale_factor;
		break;

	case SCALE_UP_BIN:
		 
		frac *= ps->user_tsresol;
		frac /= ps->ifaces[interface_id].tsresol;
		break;

	case SCALE_DOWN_DEC:
		 
		frac /= ps->ifaces[interface_id].scale_factor;
		break;


	case SCALE_DOWN_BIN:
		 
		frac *= ps->user_tsresol;
		frac /= ps->ifaces[interface_id].tsresol;
		break;
	}
#ifdef _WIN32
	 
	hdr->ts.tv_sec = (long)sec;
	hdr->ts.tv_usec = (long)frac;
#else
	 
	hdr->ts.tv_sec = (time_t)sec;
	hdr->ts.tv_usec = (int)frac;
#endif

	 
	*data = get_from_block_data(&cursor, hdr->caplen, p->errbuf);
	if (*data == NULL)
		return (-1);

	if (p->swapped)
		swap_pseudo_headers(p->linktype, hdr, *data);

	return (0);
}
