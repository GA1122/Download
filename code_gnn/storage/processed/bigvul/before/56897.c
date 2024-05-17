static int send_mpa_reject(struct iwch_ep *ep, const void *pdata, u8 plen)
{
	int mpalen;
	struct tx_data_wr *req;
	struct mpa_message *mpa;
	struct sk_buff *skb;

	PDBG("%s ep %p plen %d\n", __func__, ep, plen);

	mpalen = sizeof(*mpa) + plen;

	skb = get_skb(NULL, mpalen + sizeof(*req), GFP_KERNEL);
	if (!skb) {
		printk(KERN_ERR MOD "%s - cannot alloc skb!\n", __func__);
		return -ENOMEM;
	}
	skb_reserve(skb, sizeof(*req));
	mpa = (struct mpa_message *) skb_put(skb, mpalen);
	memset(mpa, 0, sizeof(*mpa));
	memcpy(mpa->key, MPA_KEY_REP, sizeof(mpa->key));
	mpa->flags = MPA_REJECT;
	mpa->revision = mpa_rev;
	mpa->private_data_size = htons(plen);
	if (plen)
		memcpy(mpa->private_data, pdata, plen);

	 
	skb_get(skb);
	skb->priority = CPL_PRIORITY_DATA;
	set_arp_failure_handler(skb, arp_failure_discard);
	skb_reset_transport_header(skb);
	req = (struct tx_data_wr *) skb_push(skb, sizeof(*req));
	req->wr_hi = htonl(V_WR_OP(FW_WROPCODE_OFLD_TX_DATA)|F_WR_COMPL);
	req->wr_lo = htonl(V_WR_TID(ep->hwtid));
	req->len = htonl(mpalen);
	req->param = htonl(V_TX_PORT(ep->l2t->smt_idx) |
			   V_TX_SNDBUF(snd_win>>15));
	req->flags = htonl(F_TX_INIT);
	req->sndseq = htonl(ep->snd_seq);
	BUG_ON(ep->mpa_skb);
	ep->mpa_skb = skb;
	return iwch_l2t_send(ep->com.tdev, skb, ep->l2t);
}