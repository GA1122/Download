static int rx_data(struct t3cdev *tdev, struct sk_buff *skb, void *ctx)
{
	struct iwch_ep *ep = ctx;
	struct cpl_rx_data *hdr = cplhdr(skb);
	unsigned int dlen = ntohs(hdr->len);

	PDBG("%s ep %p dlen %u\n", __func__, ep, dlen);

	skb_pull(skb, sizeof(*hdr));
	skb_trim(skb, dlen);

	ep->rcv_seq += dlen;
	BUG_ON(ep->rcv_seq != (ntohl(hdr->seq) + dlen));

	switch (state_read(&ep->com)) {
	case MPA_REQ_SENT:
		process_mpa_reply(ep, skb);
		break;
	case MPA_REQ_WAIT:
		process_mpa_request(ep, skb);
		break;
	case MPA_REP_SENT:
		break;
	default:
		printk(KERN_ERR MOD "%s Unexpected streaming data."
		       " ep %p state %d tid %d\n",
		       __func__, ep, state_read(&ep->com), ep->hwtid);

		 
		break;
	}

	 
	update_rx_credits(ep, dlen);

	return CPL_RET_BUF_DONE;
}