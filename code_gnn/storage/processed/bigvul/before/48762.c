bool __skb_csum_offload_chk(struct sk_buff *skb,
			    const struct skb_csum_offl_spec *spec,
			    bool *csum_encapped,
			    bool csum_help)
{
	struct iphdr *iph;
	struct ipv6hdr *ipv6;
	void *nhdr;
	int protocol;
	u8 ip_proto;

	if (skb->protocol == htons(ETH_P_8021Q) ||
	    skb->protocol == htons(ETH_P_8021AD)) {
		if (!spec->vlan_okay)
			goto need_help;
	}

	 
	if (skb_checksum_start_offset(skb) == skb_transport_offset(skb)) {
		 
		protocol = eproto_to_ipproto(vlan_get_protocol(skb));
		nhdr = skb_network_header(skb);
		*csum_encapped = false;
		if (spec->no_not_encapped)
			goto need_help;
	} else if (skb->encapsulation && spec->encap_okay &&
		   skb_checksum_start_offset(skb) ==
		   skb_inner_transport_offset(skb)) {
		 
		*csum_encapped = true;
		switch (skb->inner_protocol_type) {
		case ENCAP_TYPE_ETHER:
			protocol = eproto_to_ipproto(skb->inner_protocol);
			break;
		case ENCAP_TYPE_IPPROTO:
			protocol = skb->inner_protocol;
			break;
		}
		nhdr = skb_inner_network_header(skb);
	} else {
		goto need_help;
	}

	switch (protocol) {
	case IPPROTO_IP:
		if (!spec->ipv4_okay)
			goto need_help;
		iph = nhdr;
		ip_proto = iph->protocol;
		if (iph->ihl != 5 && !spec->ip_options_okay)
			goto need_help;
		break;
	case IPPROTO_IPV6:
		if (!spec->ipv6_okay)
			goto need_help;
		if (spec->no_encapped_ipv6 && *csum_encapped)
			goto need_help;
		ipv6 = nhdr;
		nhdr += sizeof(*ipv6);
		ip_proto = ipv6->nexthdr;
		break;
	default:
		goto need_help;
	}

ip_proto_again:
	switch (ip_proto) {
	case IPPROTO_TCP:
		if (!spec->tcp_okay ||
		    skb->csum_offset != offsetof(struct tcphdr, check))
			goto need_help;
		break;
	case IPPROTO_UDP:
		if (!spec->udp_okay ||
		    skb->csum_offset != offsetof(struct udphdr, check))
			goto need_help;
		break;
	case IPPROTO_SCTP:
		if (!spec->sctp_okay ||
		    skb->csum_offset != offsetof(struct sctphdr, checksum))
			goto cant_help;
		break;
	case NEXTHDR_HOP:
	case NEXTHDR_ROUTING:
	case NEXTHDR_DEST: {
		u8 *opthdr = nhdr;

		if (protocol != IPPROTO_IPV6 || !spec->ext_hdrs_okay)
			goto need_help;

		ip_proto = opthdr[0];
		nhdr += (opthdr[1] + 1) << 3;

		goto ip_proto_again;
	}
	default:
		goto need_help;
	}

	 
	return true;

need_help:
	if (csum_help && !skb_shinfo(skb)->gso_size)
		skb_checksum_help(skb);
cant_help:
	return false;
}