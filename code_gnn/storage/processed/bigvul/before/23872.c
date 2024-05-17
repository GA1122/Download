static ssize_t tun_put_user(struct tun_struct *tun,
			    struct sk_buff *skb,
			    const struct iovec *iv, int len)
{
	struct tun_pi pi = { 0, skb->protocol };
	ssize_t total = 0;

	if (!(tun->flags & TUN_NO_PI)) {
		if ((len -= sizeof(pi)) < 0)
			return -EINVAL;

		if (len < skb->len) {
			 
			pi.flags |= TUN_PKT_STRIP;
		}

		if (memcpy_toiovecend(iv, (void *) &pi, 0, sizeof(pi)))
			return -EFAULT;
		total += sizeof(pi);
	}

	if (tun->flags & TUN_VNET_HDR) {
		struct virtio_net_hdr gso = { 0 };  
		if ((len -= tun->vnet_hdr_sz) < 0)
			return -EINVAL;

		if (skb_is_gso(skb)) {
			struct skb_shared_info *sinfo = skb_shinfo(skb);

			 
			gso.hdr_len = skb_headlen(skb);
			gso.gso_size = sinfo->gso_size;
			if (sinfo->gso_type & SKB_GSO_TCPV4)
				gso.gso_type = VIRTIO_NET_HDR_GSO_TCPV4;
			else if (sinfo->gso_type & SKB_GSO_TCPV6)
				gso.gso_type = VIRTIO_NET_HDR_GSO_TCPV6;
			else if (sinfo->gso_type & SKB_GSO_UDP)
				gso.gso_type = VIRTIO_NET_HDR_GSO_UDP;
			else {
				pr_err("unexpected GSO type: "
				       "0x%x, gso_size %d, hdr_len %d\n",
				       sinfo->gso_type, gso.gso_size,
				       gso.hdr_len);
				print_hex_dump(KERN_ERR, "tun: ",
					       DUMP_PREFIX_NONE,
					       16, 1, skb->head,
					       min((int)gso.hdr_len, 64), true);
				WARN_ON_ONCE(1);
				return -EINVAL;
			}
			if (sinfo->gso_type & SKB_GSO_TCP_ECN)
				gso.gso_type |= VIRTIO_NET_HDR_GSO_ECN;
		} else
			gso.gso_type = VIRTIO_NET_HDR_GSO_NONE;

		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			gso.flags = VIRTIO_NET_HDR_F_NEEDS_CSUM;
			gso.csum_start = skb_checksum_start_offset(skb);
			gso.csum_offset = skb->csum_offset;
		} else if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
			gso.flags = VIRTIO_NET_HDR_F_DATA_VALID;
		}  

		if (unlikely(memcpy_toiovecend(iv, (void *)&gso, total,
					       sizeof(gso))))
			return -EFAULT;
		total += tun->vnet_hdr_sz;
	}

	len = min_t(int, skb->len, len);

	skb_copy_datagram_const_iovec(skb, 0, iv, total, len);
	total += skb->len;

	tun->dev->stats.tx_packets++;
	tun->dev->stats.tx_bytes += len;

	return total;
}