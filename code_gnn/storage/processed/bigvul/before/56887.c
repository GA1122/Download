static void process_mpa_request(struct iwch_ep *ep, struct sk_buff *skb)
{
	struct mpa_message *mpa;
	u16 plen;

	PDBG("%s ep %p\n", __func__, ep);

	 
	stop_ep_timer(ep);
	if (state_read(&ep->com) != MPA_REQ_WAIT)
		return;

	 
	if (ep->mpa_pkt_len + skb->len > sizeof(ep->mpa_pkt)) {
		abort_connection(ep, skb, GFP_KERNEL);
		return;
	}

	PDBG("%s enter (%s line %u)\n", __func__, __FILE__, __LINE__);

	 
	skb_copy_from_linear_data(skb, &(ep->mpa_pkt[ep->mpa_pkt_len]),
				  skb->len);
	ep->mpa_pkt_len += skb->len;

	 
	if (ep->mpa_pkt_len < sizeof(*mpa))
		return;
	PDBG("%s enter (%s line %u)\n", __func__, __FILE__, __LINE__);
	mpa = (struct mpa_message *) ep->mpa_pkt;

	 
	if (mpa->revision != mpa_rev) {
		abort_connection(ep, skb, GFP_KERNEL);
		return;
	}

	if (memcmp(mpa->key, MPA_KEY_REQ, sizeof(mpa->key))) {
		abort_connection(ep, skb, GFP_KERNEL);
		return;
	}

	plen = ntohs(mpa->private_data_size);

	 
	if (plen > MPA_MAX_PRIVATE_DATA) {
		abort_connection(ep, skb, GFP_KERNEL);
		return;
	}

	 
	if (ep->mpa_pkt_len > (sizeof(*mpa) + plen)) {
		abort_connection(ep, skb, GFP_KERNEL);
		return;
	}
	ep->plen = (u8) plen;

	 
	if (ep->mpa_pkt_len < (sizeof(*mpa) + plen))
		return;

	 
	ep->mpa_attr.initiator = 0;
	ep->mpa_attr.crc_enabled = (mpa->flags & MPA_CRC) | crc_enabled ? 1 : 0;
	ep->mpa_attr.recv_marker_enabled = markers_enabled;
	ep->mpa_attr.xmit_marker_enabled = mpa->flags & MPA_MARKERS ? 1 : 0;
	ep->mpa_attr.version = mpa_rev;
	PDBG("%s - crc_enabled=%d, recv_marker_enabled=%d, "
	     "xmit_marker_enabled=%d, version=%d\n", __func__,
	     ep->mpa_attr.crc_enabled, ep->mpa_attr.recv_marker_enabled,
	     ep->mpa_attr.xmit_marker_enabled, ep->mpa_attr.version);

	state_set(&ep->com, MPA_REQ_RCVD);

	 
	connect_request_upcall(ep);
	return;
}
