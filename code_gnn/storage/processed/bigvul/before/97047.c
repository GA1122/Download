int ip6_fragment(struct sk_buff *skb, int (*output)(struct sk_buff *))
{
	struct sk_buff *frag;
	struct rt6_info *rt = (struct rt6_info *)skb_dst(skb);
	struct ipv6_pinfo *np = skb->sk && !dev_recursion_level() ?
				inet6_sk(skb->sk) : NULL;
	struct ipv6hdr *tmp_hdr;
	struct frag_hdr *fh;
	unsigned int mtu, hlen, left, len;
	int hroom, troom;
	__be32 frag_id = 0;
	int ptr, offset = 0, err = 0;
	u8 *prevhdr, nexthdr = 0;
	struct net *net = dev_net(skb_dst(skb)->dev);

	err = ip6_find_1stfragopt(skb, &prevhdr);
	if (err < 0)
		goto fail;
	hlen = err;
	nexthdr = *prevhdr;

	mtu = ip6_skb_dst_mtu(skb);

	 
	if (unlikely(!skb->ignore_df && skb->len > mtu) ||
		     (IP6CB(skb)->frag_max_size &&
		      IP6CB(skb)->frag_max_size > mtu)) {
		if (skb->sk && dst_allfrag(skb_dst(skb)))
			sk_nocaps_add(skb->sk, NETIF_F_GSO_MASK);

		skb->dev = skb_dst(skb)->dev;
		icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
		IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
			      IPSTATS_MIB_FRAGFAILS);
		kfree_skb(skb);
		return -EMSGSIZE;
	}

	if (np && np->frag_size < mtu) {
		if (np->frag_size)
			mtu = np->frag_size;
	}
	mtu -= hlen + sizeof(struct frag_hdr);

	if (skb_has_frag_list(skb)) {
		int first_len = skb_pagelen(skb);
		struct sk_buff *frag2;

		if (first_len - hlen > mtu ||
		    ((first_len - hlen) & 7) ||
		    skb_cloned(skb))
			goto slow_path;

		skb_walk_frags(skb, frag) {
			 
			if (frag->len > mtu ||
			    ((frag->len & 7) && frag->next) ||
			    skb_headroom(frag) < hlen)
				goto slow_path_clean;

			 
			if (skb_shared(frag))
				goto slow_path_clean;

			BUG_ON(frag->sk);
			if (skb->sk) {
				frag->sk = skb->sk;
				frag->destructor = sock_wfree;
			}
			skb->truesize -= frag->truesize;
		}

		err = 0;
		offset = 0;
		frag = skb_shinfo(skb)->frag_list;
		skb_frag_list_init(skb);
		 

		*prevhdr = NEXTHDR_FRAGMENT;
		tmp_hdr = kmemdup(skb_network_header(skb), hlen, GFP_ATOMIC);
		if (!tmp_hdr) {
			IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
				      IPSTATS_MIB_FRAGFAILS);
			return -ENOMEM;
		}

		__skb_pull(skb, hlen);
		fh = (struct frag_hdr *)__skb_push(skb, sizeof(struct frag_hdr));
		__skb_push(skb, hlen);
		skb_reset_network_header(skb);
		memcpy(skb_network_header(skb), tmp_hdr, hlen);

		ipv6_select_ident(fh, rt);
		fh->nexthdr = nexthdr;
		fh->reserved = 0;
		fh->frag_off = htons(IP6_MF);
		frag_id = fh->identification;

		first_len = skb_pagelen(skb);
		skb->data_len = first_len - skb_headlen(skb);
		skb->len = first_len;
		ipv6_hdr(skb)->payload_len = htons(first_len -
						   sizeof(struct ipv6hdr));

		dst_hold(&rt->dst);

		for (;;) {
			 
			if (frag) {
				frag->ip_summed = CHECKSUM_NONE;
				skb_reset_transport_header(frag);
				fh = (struct frag_hdr *)__skb_push(frag, sizeof(struct frag_hdr));
				__skb_push(frag, hlen);
				skb_reset_network_header(frag);
				memcpy(skb_network_header(frag), tmp_hdr,
				       hlen);
				offset += skb->len - hlen - sizeof(struct frag_hdr);
				fh->nexthdr = nexthdr;
				fh->reserved = 0;
				fh->frag_off = htons(offset);
				if (frag->next != NULL)
					fh->frag_off |= htons(IP6_MF);
				fh->identification = frag_id;
				ipv6_hdr(frag)->payload_len =
						htons(frag->len -
						      sizeof(struct ipv6hdr));
				ip6_copy_metadata(frag, skb);
			}

			err = output(skb);
			if (!err)
				IP6_INC_STATS(net, ip6_dst_idev(&rt->dst),
					      IPSTATS_MIB_FRAGCREATES);

			if (err || !frag)
				break;

			skb = frag;
			frag = skb->next;
			skb->next = NULL;
		}

		kfree(tmp_hdr);

		if (err == 0) {
			IP6_INC_STATS(net, ip6_dst_idev(&rt->dst),
				      IPSTATS_MIB_FRAGOKS);
			ip6_rt_put(rt);
			return 0;
		}

		kfree_skb_list(frag);

		IP6_INC_STATS(net, ip6_dst_idev(&rt->dst),
			      IPSTATS_MIB_FRAGFAILS);
		ip6_rt_put(rt);
		return err;

slow_path_clean:
		skb_walk_frags(skb, frag2) {
			if (frag2 == frag)
				break;
			frag2->sk = NULL;
			frag2->destructor = NULL;
			skb->truesize += frag2->truesize;
		}
	}

slow_path:
	if ((skb->ip_summed == CHECKSUM_PARTIAL) &&
	    skb_checksum_help(skb))
		goto fail;

	left = skb->len - hlen;		 
	ptr = hlen;			 

	 

	*prevhdr = NEXTHDR_FRAGMENT;
	hroom = LL_RESERVED_SPACE(rt->dst.dev);
	troom = rt->dst.dev->needed_tailroom;

	 
	while (left > 0)	{
		len = left;
		 
		if (len > mtu)
			len = mtu;
		 
		if (len < left)	{
			len &= ~7;
		}
		 

		if ((frag = alloc_skb(len + hlen + sizeof(struct frag_hdr) +
				      hroom + troom, GFP_ATOMIC)) == NULL) {
			NETDEBUG(KERN_INFO "IPv6: frag: no memory for new fragment!\n");
			IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
				      IPSTATS_MIB_FRAGFAILS);
			err = -ENOMEM;
			goto fail;
		}

		 

		ip6_copy_metadata(frag, skb);
		skb_reserve(frag, hroom);
		skb_put(frag, len + hlen + sizeof(struct frag_hdr));
		skb_reset_network_header(frag);
		fh = (struct frag_hdr *)(skb_network_header(frag) + hlen);
		frag->transport_header = (frag->network_header + hlen +
					  sizeof(struct frag_hdr));

		 
		if (skb->sk)
			skb_set_owner_w(frag, skb->sk);

		 
		skb_copy_from_linear_data(skb, skb_network_header(frag), hlen);

		 
		fh->nexthdr = nexthdr;
		fh->reserved = 0;
		if (!frag_id) {
			ipv6_select_ident(fh, rt);
			frag_id = fh->identification;
		} else
			fh->identification = frag_id;

		 
		BUG_ON(skb_copy_bits(skb, ptr, skb_transport_header(frag),
				     len));
		left -= len;

		fh->frag_off = htons(offset);
		if (left > 0)
			fh->frag_off |= htons(IP6_MF);
		ipv6_hdr(frag)->payload_len = htons(frag->len -
						    sizeof(struct ipv6hdr));

		ptr += len;
		offset += len;

		 
		err = output(frag);
		if (err)
			goto fail;

		IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
			      IPSTATS_MIB_FRAGCREATES);
	}
	IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
		      IPSTATS_MIB_FRAGOKS);
	consume_skb(skb);
	return err;

fail:
	IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
		      IPSTATS_MIB_FRAGFAILS);
	kfree_skb(skb);
	return err;
}
