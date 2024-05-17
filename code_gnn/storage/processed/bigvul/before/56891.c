 static void release_tid(struct t3cdev *tdev, u32 hwtid, struct sk_buff *skb)
{
	struct cpl_tid_release *req;

	skb = get_skb(skb, sizeof *req, GFP_KERNEL);
	if (!skb)
		return;
	req = (struct cpl_tid_release *) skb_put(skb, sizeof(*req));
	req->wr.wr_hi = htonl(V_WR_OP(FW_WROPCODE_FORWARD));
	OPCODE_TID(req) = htonl(MK_OPCODE_TID(CPL_TID_RELEASE, hwtid));
	skb->priority = CPL_PRIORITY_SETUP;
	iwch_cxgb3_ofld_send(tdev, skb);
	return;
}