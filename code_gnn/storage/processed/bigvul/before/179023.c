 int ip6_append_data(struct sock *sk, int getfrag(void *from, char *to,
 	int offset, int len, int odd, struct sk_buff *skb),
 	void *from, int length, int transhdrlen,
 	int hlimit, int tclass, struct ipv6_txoptions *opt, struct flowi6 *fl6,
 	struct rt6_info *rt, unsigned int flags, int dontfrag)
 {
 	struct inet_sock *inet = inet_sk(sk);
 	struct ipv6_pinfo *np = inet6_sk(sk);
 	struct inet_cork *cork;
 	struct sk_buff *skb;
 	unsigned int maxfraglen, fragheaderlen;
 	int exthdrlen;
 	int hh_len;
 	int mtu;
 	int copy;
 	int err;
 	int offset = 0;
 	int csummode = CHECKSUM_NONE;
 	__u8 tx_flags = 0;
 
 	if (flags&MSG_PROBE)
 		return 0;
 	cork = &inet->cork.base;
 	if (skb_queue_empty(&sk->sk_write_queue)) {
 		 
 		if (opt) {
 			if (WARN_ON(np->cork.opt))
 				return -EINVAL;
 
 			np->cork.opt = kmalloc(opt->tot_len, sk->sk_allocation);
 			if (unlikely(np->cork.opt == NULL))
 				return -ENOBUFS;
 
 			np->cork.opt->tot_len = opt->tot_len;
 			np->cork.opt->opt_flen = opt->opt_flen;
 			np->cork.opt->opt_nflen = opt->opt_nflen;
 
 			np->cork.opt->dst0opt = ip6_opt_dup(opt->dst0opt,
 							    sk->sk_allocation);
 			if (opt->dst0opt && !np->cork.opt->dst0opt)
 				return -ENOBUFS;
 
 			np->cork.opt->dst1opt = ip6_opt_dup(opt->dst1opt,
 							    sk->sk_allocation);
 			if (opt->dst1opt && !np->cork.opt->dst1opt)
 				return -ENOBUFS;
 
 			np->cork.opt->hopopt = ip6_opt_dup(opt->hopopt,
 							   sk->sk_allocation);
 			if (opt->hopopt && !np->cork.opt->hopopt)
 				return -ENOBUFS;
 
 			np->cork.opt->srcrt = ip6_rthdr_dup(opt->srcrt,
 							    sk->sk_allocation);
 			if (opt->srcrt && !np->cork.opt->srcrt)
 				return -ENOBUFS;
 
 			 
 		}
 		dst_hold(&rt->dst);
 		cork->dst = &rt->dst;
 		inet->cork.fl.u.ip6 = *fl6;
 		np->cork.hop_limit = hlimit;
 		np->cork.tclass = tclass;
 		mtu = np->pmtudisc == IPV6_PMTUDISC_PROBE ?
 		      rt->dst.dev->mtu : dst_mtu(rt->dst.path);
 		if (np->frag_size < mtu) {
 			if (np->frag_size)
 				mtu = np->frag_size;
 		}
 		cork->fragsize = mtu;
 		if (dst_allfrag(rt->dst.path))
 			cork->flags |= IPCORK_ALLFRAG;
 		cork->length = 0;
 		sk->sk_sndmsg_page = NULL;
 		sk->sk_sndmsg_off = 0;
 		exthdrlen = rt->dst.header_len + (opt ? opt->opt_flen : 0) -
 			    rt->rt6i_nfheader_len;
 		length += exthdrlen;
 		transhdrlen += exthdrlen;
 	} else {
 		rt = (struct rt6_info *)cork->dst;
 		fl6 = &inet->cork.fl.u.ip6;
 		opt = np->cork.opt;
 		transhdrlen = 0;
 		exthdrlen = 0;
 		mtu = cork->fragsize;
 	}
 
 	hh_len = LL_RESERVED_SPACE(rt->dst.dev);
 
 	fragheaderlen = sizeof(struct ipv6hdr) + rt->rt6i_nfheader_len +
 			(opt ? opt->opt_nflen : 0);
 	maxfraglen = ((mtu - fragheaderlen) & ~7) + fragheaderlen - sizeof(struct frag_hdr);
 
 	if (mtu <= sizeof(struct ipv6hdr) + IPV6_MAXPLEN) {
 		if (cork->length + length > sizeof(struct ipv6hdr) + IPV6_MAXPLEN - fragheaderlen) {
 			ipv6_local_error(sk, EMSGSIZE, fl6, mtu-exthdrlen);
 			return -EMSGSIZE;
 		}
 	}
 
 	 
 	if (sk->sk_type == SOCK_DGRAM) {
 		err = sock_tx_timestamp(sk, &tx_flags);
 		if (err)
 			goto error;
 	}
 
 	 
 
 	cork->length += length;
 	if (length > mtu) {
 		int proto = sk->sk_protocol;
 		if (dontfrag && (proto == IPPROTO_UDP || proto == IPPROTO_RAW)){
 			ipv6_local_rxpmtu(sk, fl6, mtu-exthdrlen);
 			return -EMSGSIZE;
 		}
 
 		if (proto == IPPROTO_UDP &&
 		    (rt->dst.dev->features & NETIF_F_UFO)) {
  
  			err = ip6_ufo_append_data(sk, getfrag, from, length,
  						  hh_len, fragheaderlen,
						  transhdrlen, mtu, flags);
// 						  transhdrlen, mtu, flags, rt);
  			if (err)
  				goto error;
  			return 0;
 		}
 	}
 
 	if ((skb = skb_peek_tail(&sk->sk_write_queue)) == NULL)
 		goto alloc_new_skb;
 
 	while (length > 0) {
 		 
 		copy = (cork->length <= mtu && !(cork->flags & IPCORK_ALLFRAG) ? mtu : maxfraglen) - skb->len;
 		if (copy < length)
 			copy = maxfraglen - skb->len;
 
 		if (copy <= 0) {
 			char *data;
 			unsigned int datalen;
 			unsigned int fraglen;
 			unsigned int fraggap;
 			unsigned int alloclen;
 			struct sk_buff *skb_prev;
 alloc_new_skb:
 			skb_prev = skb;
 
 			 
 			if (skb_prev)
 				fraggap = skb_prev->len - maxfraglen;
 			else
 				fraggap = 0;
 
 			 
 			datalen = length + fraggap;
 			if (datalen > (cork->length <= mtu && !(cork->flags & IPCORK_ALLFRAG) ? mtu : maxfraglen) - fragheaderlen)
 				datalen = maxfraglen - fragheaderlen;
 
 			fraglen = datalen + fragheaderlen;
 			if ((flags & MSG_MORE) &&
 			    !(rt->dst.dev->features&NETIF_F_SG))
 				alloclen = mtu;
 			else
 				alloclen = datalen + fragheaderlen;
 
 			 
 			if (datalen == length + fraggap)
 				alloclen += rt->dst.trailer_len;
 
 			 
 			alloclen += sizeof(struct frag_hdr);
 
 			if (transhdrlen) {
 				skb = sock_alloc_send_skb(sk,
 						alloclen + hh_len,
 						(flags & MSG_DONTWAIT), &err);
 			} else {
 				skb = NULL;
 				if (atomic_read(&sk->sk_wmem_alloc) <=
 				    2 * sk->sk_sndbuf)
 					skb = sock_wmalloc(sk,
 							   alloclen + hh_len, 1,
 							   sk->sk_allocation);
 				if (unlikely(skb == NULL))
 					err = -ENOBUFS;
 				else {
 					 
 					tx_flags = 0;
 				}
 			}
 			if (skb == NULL)
 				goto error;
 			 
 			skb->ip_summed = csummode;
 			skb->csum = 0;
 			 
 			skb_reserve(skb, hh_len+sizeof(struct frag_hdr));
 
 			if (sk->sk_type == SOCK_DGRAM)
 				skb_shinfo(skb)->tx_flags = tx_flags;
 
 			 
 			data = skb_put(skb, fraglen);
 			skb_set_network_header(skb, exthdrlen);
 			data += fragheaderlen;
 			skb->transport_header = (skb->network_header +
 						 fragheaderlen);
 			if (fraggap) {
 				skb->csum = skb_copy_and_csum_bits(
 					skb_prev, maxfraglen,
 					data + transhdrlen, fraggap, 0);
 				skb_prev->csum = csum_sub(skb_prev->csum,
 							  skb->csum);
 				data += fraggap;
 				pskb_trim_unique(skb_prev, maxfraglen);
 			}
 			copy = datalen - transhdrlen - fraggap;
 			if (copy < 0) {
 				err = -EINVAL;
 				kfree_skb(skb);
 				goto error;
 			} else if (copy > 0 && getfrag(from, data + transhdrlen, offset, copy, fraggap, skb) < 0) {
 				err = -EFAULT;
 				kfree_skb(skb);
 				goto error;
 			}
 
 			offset += copy;
 			length -= datalen - fraggap;
 			transhdrlen = 0;
 			exthdrlen = 0;
 			csummode = CHECKSUM_NONE;
 
 			 
 			__skb_queue_tail(&sk->sk_write_queue, skb);
 			continue;
 		}
 
 		if (copy > length)
 			copy = length;
 
 		if (!(rt->dst.dev->features&NETIF_F_SG)) {
 			unsigned int off;
 
 			off = skb->len;
 			if (getfrag(from, skb_put(skb, copy),
 						offset, copy, off, skb) < 0) {
 				__skb_trim(skb, off);
 				err = -EFAULT;
 				goto error;
 			}
 		} else {
 			int i = skb_shinfo(skb)->nr_frags;
 			skb_frag_t *frag = &skb_shinfo(skb)->frags[i-1];
 			struct page *page = sk->sk_sndmsg_page;
 			int off = sk->sk_sndmsg_off;
 			unsigned int left;
 
 			if (page && (left = PAGE_SIZE - off) > 0) {
 				if (copy >= left)
 					copy = left;
 				if (page != frag->page) {
 					if (i == MAX_SKB_FRAGS) {
 						err = -EMSGSIZE;
 						goto error;
 					}
 					get_page(page);
 					skb_fill_page_desc(skb, i, page, sk->sk_sndmsg_off, 0);
 					frag = &skb_shinfo(skb)->frags[i];
 				}
 			} else if(i < MAX_SKB_FRAGS) {
 				if (copy > PAGE_SIZE)
 					copy = PAGE_SIZE;
 				page = alloc_pages(sk->sk_allocation, 0);
 				if (page == NULL) {
 					err = -ENOMEM;
 					goto error;
 				}
 				sk->sk_sndmsg_page = page;
 				sk->sk_sndmsg_off = 0;
 
 				skb_fill_page_desc(skb, i, page, 0, 0);
 				frag = &skb_shinfo(skb)->frags[i];
 			} else {
 				err = -EMSGSIZE;
 				goto error;
 			}
 			if (getfrag(from, page_address(frag->page)+frag->page_offset+frag->size, offset, copy, skb->len, skb) < 0) {
 				err = -EFAULT;
 				goto error;
 			}
 			sk->sk_sndmsg_off += copy;
 			frag->size += copy;
 			skb->len += copy;
 			skb->data_len += copy;
 			skb->truesize += copy;
 			atomic_add(copy, &sk->sk_wmem_alloc);
 		}
 		offset += copy;
 		length -= copy;
 	}
 	return 0;
 error:
 	cork->length -= length;
 	IP6_INC_STATS(sock_net(sk), rt->rt6i_idev, IPSTATS_MIB_OUTDISCARDS);
 	return err;
 }