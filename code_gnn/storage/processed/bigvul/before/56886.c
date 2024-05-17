static void process_mpa_reply(struct iwch_ep *ep, struct sk_buff *skb)
{
	struct mpa_message *mpa;
	u16 plen;
	struct iwch_qp_attributes attrs;
	enum iwch_qp_attr_mask mask;
	int err;

	PDBG("%s ep %p\n", __func__, ep);

	 
	stop_ep_timer(ep);
	if (state_read(&ep->com) != MPA_REQ_SENT)
		return;

	 
	if (ep->mpa_pkt_len + skb->len > sizeof(ep->mpa_pkt)) {
		err = -EINVAL;
		goto err;
	}

	 
	skb_copy_from_linear_data(skb, &(ep->mpa_pkt[ep->mpa_pkt_len]),
				  skb->len);
	ep->mpa_pkt_len += skb->len;

	 
	if (ep->mpa_pkt_len < sizeof(*mpa))
		return;
	mpa = (struct mpa_message *) ep->mpa_pkt;

	 
	if (mpa->revision != mpa_rev) {
		err = -EPROTO;
		goto err;
	}
	if (memcmp(mpa->key, MPA_KEY_REP, sizeof(mpa->key))) {
		err = -EPROTO;
		goto err;
	}

	plen = ntohs(mpa->private_data_size);

	 
	if (plen > MPA_MAX_PRIVATE_DATA) {
		err = -EPROTO;
		goto err;
	}

	 
	if (ep->mpa_pkt_len > (sizeof(*mpa) + plen)) {
		err = -EPROTO;
		goto err;
	}

	ep->plen = (u8) plen;

	 
	if (ep->mpa_pkt_len < (sizeof(*mpa) + plen))
		return;

	if (mpa->flags & MPA_REJECT) {
		err = -ECONNREFUSED;
		goto err;
	}

	 
	state_set(&ep->com, FPDU_MODE);
	ep->mpa_attr.initiator = 1;
	ep->mpa_attr.crc_enabled = (mpa->flags & MPA_CRC) | crc_enabled ? 1 : 0;
	ep->mpa_attr.recv_marker_enabled = markers_enabled;
	ep->mpa_attr.xmit_marker_enabled = mpa->flags & MPA_MARKERS ? 1 : 0;
	ep->mpa_attr.version = mpa_rev;
	PDBG("%s - crc_enabled=%d, recv_marker_enabled=%d, "
	     "xmit_marker_enabled=%d, version=%d\n", __func__,
	     ep->mpa_attr.crc_enabled, ep->mpa_attr.recv_marker_enabled,
	     ep->mpa_attr.xmit_marker_enabled, ep->mpa_attr.version);

	attrs.mpa_attr = ep->mpa_attr;
	attrs.max_ird = ep->ird;
	attrs.max_ord = ep->ord;
	attrs.llp_stream_handle = ep;
	attrs.next_state = IWCH_QP_STATE_RTS;

	mask = IWCH_QP_ATTR_NEXT_STATE |
	    IWCH_QP_ATTR_LLP_STREAM_HANDLE | IWCH_QP_ATTR_MPA_ATTR |
	    IWCH_QP_ATTR_MAX_IRD | IWCH_QP_ATTR_MAX_ORD;

	 
	err = iwch_modify_qp(ep->com.qp->rhp,
			     ep->com.qp, mask, &attrs, 1);
	if (err)
		goto err;

	if (peer2peer && iwch_rqes_posted(ep->com.qp) == 0) {
		iwch_post_zb_read(ep);
	}

	goto out;
err:
	abort_connection(ep, skb, GFP_KERNEL);
out:
	connect_reply_upcall(ep, err);
	return;
}
