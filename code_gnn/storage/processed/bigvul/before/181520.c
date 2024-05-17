 void llc_conn_handler(struct llc_sap *sap, struct sk_buff *skb)
 {
 	struct llc_addr saddr, daddr;
 	struct sock *sk;
 
 	llc_pdu_decode_sa(skb, saddr.mac);
 	llc_pdu_decode_ssap(skb, &saddr.lsap);
 	llc_pdu_decode_da(skb, daddr.mac);
 	llc_pdu_decode_dsap(skb, &daddr.lsap);
 
 	sk = __llc_lookup(sap, &saddr, &daddr);
 	if (!sk)
 		goto drop;
 
 	bh_lock_sock(sk);
 	 
 	if (unlikely(sk->sk_state == TCP_LISTEN)) {
 		struct sock *newsk = llc_create_incoming_sock(sk, skb->dev,
 							      &saddr, &daddr);
 		if (!newsk)
 			goto drop_unlock;
 		skb_set_owner_r(skb, newsk);
 	} else {
 		 
// 		skb_orphan(skb);
// 		sock_hold(sk);
  		skb->sk = sk;
// 		skb->destructor = sock_efree;
  	}
  	if (!sock_owned_by_user(sk))
  		llc_conn_rcv(sk, skb);
 	else {
 		dprintk("%s: adding to backlog...\n", __func__);
 		llc_set_backlog_type(skb, LLC_PACKET);
 		if (sk_add_backlog(sk, skb, sk->sk_rcvbuf))
 			goto drop_unlock;
 	}
 out:
 	bh_unlock_sock(sk);
 	sock_put(sk);
 	return;
 drop:
 	kfree_skb(skb);
 	return;
 drop_unlock:
 	kfree_skb(skb);
 	goto out;
 }