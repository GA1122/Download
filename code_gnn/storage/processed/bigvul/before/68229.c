static void llc_sap_state_process(struct llc_sap *sap, struct sk_buff *skb)
{
	struct llc_sap_state_ev *ev = llc_sap_ev(skb);

	 
	skb_get(skb);
	ev->ind_cfm_flag = 0;
	llc_sap_next_state(sap, skb);
	if (ev->ind_cfm_flag == LLC_IND) {
		if (skb->sk->sk_state == TCP_LISTEN)
			kfree_skb(skb);
		else {
			llc_save_primitive(skb->sk, skb, ev->prim);

			 
			if (sock_queue_rcv_skb(skb->sk, skb))
				kfree_skb(skb);
		}
	}
	kfree_skb(skb);
}