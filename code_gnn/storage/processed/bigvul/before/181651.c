 int udp_recvmsg(struct sock *sk, struct msghdr *msg, size_t len, int noblock,
 		int flags, int *addr_len)
 {
 	struct inet_sock *inet = inet_sk(sk);
 	DECLARE_SOCKADDR(struct sockaddr_in *, sin, msg->msg_name);
 	struct sk_buff *skb;
 	unsigned int ulen, copied;
  	int peeked, off = 0;
  	int err;
  	int is_udplite = IS_UDPLITE(sk);
// 	bool checksum_valid = false;
  	bool slow;
  
  	if (flags & MSG_ERRQUEUE)
 		return ip_recv_error(sk, msg, len, addr_len);
 
 try_again:
 	skb = __skb_recv_datagram(sk, flags | (noblock ? MSG_DONTWAIT : 0),
 				  &peeked, &off, &err);
 	if (!skb)
 		goto out;
 
 	ulen = skb->len - sizeof(struct udphdr);
 	copied = len;
 	if (copied > ulen)
 		copied = ulen;
 	else if (copied < ulen)
 		msg->msg_flags |= MSG_TRUNC;
 
 	 
  
  	if (copied < ulen || UDP_SKB_CB(skb)->partial_cov) {
		if (udp_lib_checksum_complete(skb))
// 		checksum_valid = !udp_lib_checksum_complete(skb);
// 		if (!checksum_valid)
  			goto csum_copy_err;
  	}
  
	if (skb_csum_unnecessary(skb))
// 	if (checksum_valid || skb_csum_unnecessary(skb))
  		err = skb_copy_datagram_msg(skb, sizeof(struct udphdr),
  					    msg, copied);
  	else {
 		err = skb_copy_and_csum_datagram_msg(skb, sizeof(struct udphdr),
 						     msg);
 
 		if (err == -EINVAL)
 			goto csum_copy_err;
 	}
 
 	if (unlikely(err)) {
 		trace_kfree_skb(skb, udp_recvmsg);
 		if (!peeked) {
 			atomic_inc(&sk->sk_drops);
 			UDP_INC_STATS_USER(sock_net(sk),
 					   UDP_MIB_INERRORS, is_udplite);
 		}
 		goto out_free;
 	}
 
 	if (!peeked)
 		UDP_INC_STATS_USER(sock_net(sk),
 				UDP_MIB_INDATAGRAMS, is_udplite);
 
 	sock_recv_ts_and_drops(msg, sk, skb);
 
 	 
 	if (sin) {
 		sin->sin_family = AF_INET;
 		sin->sin_port = udp_hdr(skb)->source;
 		sin->sin_addr.s_addr = ip_hdr(skb)->saddr;
 		memset(sin->sin_zero, 0, sizeof(sin->sin_zero));
 		*addr_len = sizeof(*sin);
 	}
 	if (inet->cmsg_flags)
 		ip_cmsg_recv_offset(msg, skb, sizeof(struct udphdr));
 
 	err = copied;
 	if (flags & MSG_TRUNC)
 		err = ulen;
 
 out_free:
 	skb_free_datagram_locked(sk, skb);
 out:
 	return err;
 
 csum_copy_err:
 	slow = lock_sock_fast(sk);
 	if (!skb_kill_datagram(sk, skb, flags)) {
 		UDP_INC_STATS_USER(sock_net(sk), UDP_MIB_CSUMERRORS, is_udplite);
 		UDP_INC_STATS_USER(sock_net(sk), UDP_MIB_INERRORS, is_udplite);
 	}
 	unlock_sock_fast(sk, slow);
 
 	 
 	cond_resched();
 	msg->msg_flags &= ~MSG_TRUNC;
 	goto try_again;
 }