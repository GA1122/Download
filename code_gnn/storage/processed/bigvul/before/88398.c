add_interface(pcap_t *p, struct block_cursor *cursor, char *errbuf)
{
	struct pcap_ng_sf *ps;
	uint64_t tsresol;
	uint64_t tsoffset;
	int is_binary;

	ps = p->priv;

	 
	ps->ifcount++;

	 
	if (ps->ifcount > ps->ifaces_size) {
		 
		bpf_u_int32 new_ifaces_size;
		struct pcap_ng_if *new_ifaces;

		if (ps->ifaces_size == 0) {
			 
			new_ifaces_size = 1;
			new_ifaces = malloc(sizeof (struct pcap_ng_if));
		} else {
			 
			if (ps->ifaces_size * 2 < ps->ifaces_size) {
				 
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "more than %u interfaces in the file",
				    0x80000000U);
				return (0);
			}

			 
			new_ifaces_size = ps->ifaces_size * 2;

			 
			if (new_ifaces_size * sizeof (struct pcap_ng_if) < new_ifaces_size) {
				 
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "more than %u interfaces in the file",
				    0xFFFFFFFFU / ((u_int)sizeof (struct pcap_ng_if)));
				return (0);
			}
			new_ifaces = realloc(ps->ifaces, new_ifaces_size * sizeof (struct pcap_ng_if));
		}
		if (new_ifaces == NULL) {
			 
			pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
			    "out of memory for per-interface information (%u interfaces)",
			    ps->ifcount);
			return (0);
		}
		ps->ifaces_size = new_ifaces_size;
		ps->ifaces = new_ifaces;
	}

	 
	tsresol = 1000000;	 
	is_binary = 0;		 
	tsoffset = 0;		 

	 
	if (process_idb_options(p, cursor, &tsresol, &tsoffset, &is_binary,
	    errbuf) == -1)
		return (0);

	ps->ifaces[ps->ifcount - 1].tsresol = tsresol;
	ps->ifaces[ps->ifcount - 1].tsoffset = tsoffset;

	 
	if (tsresol == ps->user_tsresol) {
		 
		ps->ifaces[ps->ifcount - 1].scale_type = PASS_THROUGH;
	} else if (tsresol > ps->user_tsresol) {
		 
		if (is_binary)
			ps->ifaces[ps->ifcount - 1].scale_type = SCALE_DOWN_BIN;
		else {
			 
			ps->ifaces[ps->ifcount - 1].scale_factor = tsresol/ps->user_tsresol;
			ps->ifaces[ps->ifcount - 1].scale_type = SCALE_DOWN_DEC;
		}
	} else {
		 
		if (is_binary)
			ps->ifaces[ps->ifcount - 1].scale_type = SCALE_UP_BIN;
		else {
			 
			ps->ifaces[ps->ifcount - 1].scale_factor = ps->user_tsresol/tsresol;
			ps->ifaces[ps->ifcount - 1].scale_type = SCALE_UP_DEC;
		}
	}
	return (1);
}
