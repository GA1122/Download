void llc_save_primitive(struct sock *sk, struct sk_buff *skb, u8 prim)
{
	struct sockaddr_llc *addr;

        
	addr		  = llc_ui_skb_cb(skb);

	memset(addr, 0, sizeof(*addr));
	addr->sllc_family = sk->sk_family;
	addr->sllc_arphrd = skb->dev->type;
	addr->sllc_test   = prim == LLC_TEST_PRIM;
	addr->sllc_xid    = prim == LLC_XID_PRIM;
	addr->sllc_ua     = prim == LLC_DATAUNIT_PRIM;
	llc_pdu_decode_sa(skb, addr->sllc_mac);
	llc_pdu_decode_ssap(skb, &addr->sllc_sap);
}