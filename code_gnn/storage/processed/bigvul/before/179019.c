 static int packet_recvmsg(struct kiocb *iocb, struct socket *sock,
 			  struct msghdr *msg, size_t len, int flags)
 {
 	struct sock *sk = sock->sk;
 	struct sk_buff *skb;
 	int copied, err;
 	struct sockaddr_ll *sll;
 	int vnet_hdr_len = 0;
 
 	err = -EINVAL;
 	if (flags & ~(MSG_PEEK|MSG_DONTWAIT|MSG_TRUNC|MSG_CMSG_COMPAT|MSG_ERRQUEUE))
 		goto out;
 
 #if 0
 	 
 	if (pkt_sk(sk)->ifindex < 0)
 		return -ENODEV;
 #endif
 
 	if (flags & MSG_ERRQUEUE) {
 		err = packet_recv_error(sk, msg, len);
 		goto out;
 	}
 
 	 
 
 	skb = skb_recv_datagram(sk, flags, flags & MSG_DONTWAIT, &err);
 
 	 
 
 	if (skb == NULL)
 		goto out;
 
 	if (pkt_sk(sk)->has_vnet_hdr) {
 		struct virtio_net_hdr vnet_hdr = { 0 };
 
 		err = -EINVAL;
 		vnet_hdr_len = sizeof(vnet_hdr);
 		if (len < vnet_hdr_len)
 			goto out_free;
 
 		len -= vnet_hdr_len;
 
 		if (skb_is_gso(skb)) {
 			struct skb_shared_info *sinfo = skb_shinfo(skb);
 
 			 
 			vnet_hdr.hdr_len = skb_headlen(skb);
 			vnet_hdr.gso_size = sinfo->gso_size;
 			if (sinfo->gso_type & SKB_GSO_TCPV4)
 				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_TCPV4;
 			else if (sinfo->gso_type & SKB_GSO_TCPV6)
 				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_TCPV6;
 			else if (sinfo->gso_type & SKB_GSO_UDP)
 				vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_UDP;
 			else if (sinfo->gso_type & SKB_GSO_FCOE)
 				goto out_free;
 			else
 				BUG();
 			if (sinfo->gso_type & SKB_GSO_TCP_ECN)
 				vnet_hdr.gso_type |= VIRTIO_NET_HDR_GSO_ECN;
 		} else
 			vnet_hdr.gso_type = VIRTIO_NET_HDR_GSO_NONE;
 
 		if (skb->ip_summed == CHECKSUM_PARTIAL) {
 			vnet_hdr.flags = VIRTIO_NET_HDR_F_NEEDS_CSUM;
 			vnet_hdr.csum_start = skb_checksum_start_offset(skb);
 			vnet_hdr.csum_offset = skb->csum_offset;
 		}  
 
 		err = memcpy_toiovec(msg->msg_iov, (void *)&vnet_hdr,
 				     vnet_hdr_len);
 		if (err < 0)
 			goto out_free;
 	}
 
 	 
 
 	sll = &PACKET_SKB_CB(skb)->sa.ll;
 	if (sock->type == SOCK_PACKET)
 		msg->msg_namelen = sizeof(struct sockaddr_pkt);
 	else
 		msg->msg_namelen = sll->sll_halen + offsetof(struct sockaddr_ll, sll_addr);
 
 	 
 
 	copied = skb->len;
 	if (copied > len) {
 		copied = len;
 		msg->msg_flags |= MSG_TRUNC;
 	}
 
 	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
 	if (err)
 		goto out_free;
 
 	sock_recv_ts_and_drops(msg, sk, skb);
 
 	if (msg->msg_name)
 		memcpy(msg->msg_name, &PACKET_SKB_CB(skb)->sa,
 		       msg->msg_namelen);
 
 	if (pkt_sk(sk)->auxdata) {
 		struct tpacket_auxdata aux;
 
 		aux.tp_status = TP_STATUS_USER;
 		if (skb->ip_summed == CHECKSUM_PARTIAL)
 			aux.tp_status |= TP_STATUS_CSUMNOTREADY;
 		aux.tp_len = PACKET_SKB_CB(skb)->origlen;
 		aux.tp_snaplen = skb->len;
 		aux.tp_mac = 0;
 		aux.tp_net = skb_network_offset(skb);
 		if (vlan_tx_tag_present(skb)) {
 			aux.tp_vlan_tci = vlan_tx_tag_get(skb);
 			aux.tp_status |= TP_STATUS_VLAN_VALID;
  		} else {
  			aux.tp_vlan_tci = 0;
  		}
// 		aux.tp_padding = 0;
  		put_cmsg(msg, SOL_PACKET, PACKET_AUXDATA, sizeof(aux), &aux);
  	}
  
 	 
 	err = vnet_hdr_len + ((flags&MSG_TRUNC) ? skb->len : copied);
 
 out_free:
 	skb_free_datagram(sk, skb);
 out:
 	return err;
 }