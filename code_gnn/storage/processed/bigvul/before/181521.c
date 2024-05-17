 static void llc_sap_rcv(struct llc_sap *sap, struct sk_buff *skb,
 			struct sock *sk)
 {
 	struct llc_sap_state_ev *ev = llc_sap_ev(skb);
  
  	ev->type   = LLC_SAP_EV_TYPE_PDU;
  	ev->reason = 0;
// 	skb_orphan(skb);
// 	sock_hold(sk);
  	skb->sk = sk;
// 	skb->destructor = sock_efree;
  	llc_sap_state_process(sap, skb);
  }