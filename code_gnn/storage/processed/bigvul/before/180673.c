 void sctp_generate_proto_unreach_event(unsigned long data)
  {
  	struct sctp_transport *transport = (struct sctp_transport *) data;
  	struct sctp_association *asoc = transport->asoc;
	struct net *net = sock_net(asoc->base.sk);
// 	struct sock *sk = asoc->base.sk;
// 	struct net *net = sock_net(sk);
  
	bh_lock_sock(asoc->base.sk);
	if (sock_owned_by_user(asoc->base.sk)) {
// 	bh_lock_sock(sk);
// 	if (sock_owned_by_user(sk)) {
  		pr_debug("%s: sock is busy\n", __func__);
  
  		 
 		if (!mod_timer(&transport->proto_unreach_timer,
 				jiffies + (HZ/20)))
 			sctp_association_hold(asoc);
 		goto out_unlock;
 	}
 
 	 
 	if (asoc->base.dead)
 		goto out_unlock;
 
 	sctp_do_sm(net, SCTP_EVENT_T_OTHER,
 		   SCTP_ST_OTHER(SCTP_EVENT_ICMP_PROTO_UNREACH),
  		   asoc->state, asoc->ep, asoc, transport, GFP_ATOMIC);
  
  out_unlock:
	bh_unlock_sock(asoc->base.sk);
// 	bh_unlock_sock(sk);
  	sctp_association_put(asoc);
  }