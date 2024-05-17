 static inline int ip_ufo_append_data(struct sock *sk,
 			struct sk_buff_head *queue,
 			int getfrag(void *from, char *to, int offset, int len,
 			       int odd, struct sk_buff *skb),
 			void *from, int length, int hh_len, int fragheaderlen,
 			int transhdrlen, int maxfraglen, unsigned int flags)
 {
 	struct sk_buff *skb;
 	int err;
 
 	 
 	if ((skb = skb_peek_tail(queue)) == NULL) {
 		skb = sock_alloc_send_skb(sk,
 			hh_len + fragheaderlen + transhdrlen + 20,
 			(flags & MSG_DONTWAIT), &err);
 
 		if (skb == NULL)
 			return err;
 
 		 
 		skb_reserve(skb, hh_len);
 
 		 
 		skb_put(skb, fragheaderlen + transhdrlen);
 
 		 
 		skb_reset_network_header(skb);
 
  		 
  		skb->transport_header = skb->network_header + fragheaderlen;
  
		skb->ip_summed = CHECKSUM_PARTIAL;
  		skb->csum = 0;
  
		 
		skb_shinfo(skb)->gso_size = maxfraglen - fragheaderlen;
		skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
// 
  		__skb_queue_tail(queue, skb);
// 	} else if (skb_is_gso(skb)) {
// 		goto append;
  	}
  
// 	skb->ip_summed = CHECKSUM_PARTIAL;
// 	 
// 	skb_shinfo(skb)->gso_size = maxfraglen - fragheaderlen;
// 	skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
// 
// append:
  	return skb_append_datato_frags(sk, skb, getfrag, from,
  				       (length - transhdrlen));
  }