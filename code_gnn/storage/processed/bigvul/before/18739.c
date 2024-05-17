int dccp_invalid_packet(struct sk_buff *skb)
{
	const struct dccp_hdr *dh;
	unsigned int cscov;

	if (skb->pkt_type != PACKET_HOST)
		return 1;

	 
	if (!pskb_may_pull(skb, sizeof(struct dccp_hdr))) {
		DCCP_WARN("pskb_may_pull failed\n");
		return 1;
	}

	dh = dccp_hdr(skb);

	 
	if (dh->dccph_type >= DCCP_PKT_INVALID) {
		DCCP_WARN("invalid packet type\n");
		return 1;
	}

	 
	if (dh->dccph_doff < dccp_hdr_len(skb) / sizeof(u32)) {
		DCCP_WARN("P.Data Offset(%u) too small\n", dh->dccph_doff);
		return 1;
	}
	 
	if (!pskb_may_pull(skb, dh->dccph_doff * sizeof(u32))) {
		DCCP_WARN("P.Data Offset(%u) too large\n", dh->dccph_doff);
		return 1;
	}

	 
	if ((dh->dccph_type < DCCP_PKT_DATA    ||
	    dh->dccph_type > DCCP_PKT_DATAACK) && dh->dccph_x == 0)  {
		DCCP_WARN("P.type (%s) not Data || [Data]Ack, while P.X == 0\n",
			  dccp_packet_name(dh->dccph_type));
		return 1;
	}

	 
	cscov = dccp_csum_coverage(skb);
	if (cscov > skb->len) {
		DCCP_WARN("P.CsCov %u exceeds packet length %d\n",
			  dh->dccph_cscov, skb->len);
		return 1;
	}

	 
	skb->csum = skb_checksum(skb, 0, cscov, 0);

	return 0;
}
