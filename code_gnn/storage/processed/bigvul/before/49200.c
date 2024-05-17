static int packet_recvmsg(struct socket *sock, struct msghdr *msg, size_t len,
			  int flags)
{
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
	int copied, err;
	int vnet_hdr_len = 0;
	unsigned int origlen = 0;

	err = -EINVAL;
	if (flags & ~(MSG_PEEK|MSG_DONTWAIT|MSG_TRUNC|MSG_CMSG_COMPAT|MSG_ERRQUEUE))
		goto out;

#if 0
	 
	if (pkt_sk(sk)->ifindex < 0)
		return -ENODEV;
#endif

	if (flags & MSG_ERRQUEUE) {
		err = sock_recv_errqueue(sk, msg, len,
					 SOL_PACKET, PACKET_TX_TIMESTAMP);
		goto out;
	}

	 

	skb = skb_recv_datagram(sk, flags, flags & MSG_DONTWAIT, &err);

	 

	if (skb == NULL)
		goto out;

	if (pkt_sk(sk)->pressure)
		packet_rcv_has_room(pkt_sk(sk), NULL);

	if (pkt_sk(sk)->has_vnet_hdr) {
		err = packet_rcv_vnet(msg, skb, &len);
		if (err)
			goto out_free;
		vnet_hdr_len = sizeof(struct virtio_net_hdr);
	}

	 
	copied = skb->len;
	if (copied > len) {
		copied = len;
		msg->msg_flags |= MSG_TRUNC;
	}

	err = skb_copy_datagram_msg(skb, 0, msg, copied);
	if (err)
		goto out_free;

	if (sock->type != SOCK_PACKET) {
		struct sockaddr_ll *sll = &PACKET_SKB_CB(skb)->sa.ll;

		 
		origlen = PACKET_SKB_CB(skb)->sa.origlen;
		sll->sll_family = AF_PACKET;
		sll->sll_protocol = skb->protocol;
	}

	sock_recv_ts_and_drops(msg, sk, skb);

	if (msg->msg_name) {
		 
		if (sock->type == SOCK_PACKET) {
			__sockaddr_check_size(sizeof(struct sockaddr_pkt));
			msg->msg_namelen = sizeof(struct sockaddr_pkt);
		} else {
			struct sockaddr_ll *sll = &PACKET_SKB_CB(skb)->sa.ll;

			msg->msg_namelen = sll->sll_halen +
				offsetof(struct sockaddr_ll, sll_addr);
		}
		memcpy(msg->msg_name, &PACKET_SKB_CB(skb)->sa,
		       msg->msg_namelen);
	}

	if (pkt_sk(sk)->auxdata) {
		struct tpacket_auxdata aux;

		aux.tp_status = TP_STATUS_USER;
		if (skb->ip_summed == CHECKSUM_PARTIAL)
			aux.tp_status |= TP_STATUS_CSUMNOTREADY;
		else if (skb->pkt_type != PACKET_OUTGOING &&
			 (skb->ip_summed == CHECKSUM_COMPLETE ||
			  skb_csum_unnecessary(skb)))
			aux.tp_status |= TP_STATUS_CSUM_VALID;

		aux.tp_len = origlen;
		aux.tp_snaplen = skb->len;
		aux.tp_mac = 0;
		aux.tp_net = skb_network_offset(skb);
		if (skb_vlan_tag_present(skb)) {
			aux.tp_vlan_tci = skb_vlan_tag_get(skb);
			aux.tp_vlan_tpid = ntohs(skb->vlan_proto);
			aux.tp_status |= TP_STATUS_VLAN_VALID | TP_STATUS_VLAN_TPID_VALID;
		} else {
			aux.tp_vlan_tci = 0;
			aux.tp_vlan_tpid = 0;
		}
		put_cmsg(msg, SOL_PACKET, PACKET_AUXDATA, sizeof(aux), &aux);
	}

	 
	err = vnet_hdr_len + ((flags&MSG_TRUNC) ? skb->len : copied);

out_free:
	skb_free_datagram(sk, skb);
out:
	return err;
}