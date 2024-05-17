 int ip_options_echo(struct ip_options * dopt, struct sk_buff * skb)
// int ip_options_echo(struct ip_options *dopt, struct sk_buff *skb)
  {
	struct ip_options *sopt;
// 	const struct ip_options *sopt;
  	unsigned char *sptr, *dptr;
  	int soffset, doffset;
  	int	optlen;
 	__be32	daddr;
 
 	memset(dopt, 0, sizeof(struct ip_options));
  
  	sopt = &(IPCB(skb)->opt);
  
	if (sopt->optlen == 0) {
		dopt->optlen = 0;
// 	if (sopt->optlen == 0)
  		return 0;
	}
  
  	sptr = skb_network_header(skb);
  	dptr = dopt->__data;
 
 	daddr = skb_rtable(skb)->rt_spec_dst;
 
 	if (sopt->rr) {
 		optlen  = sptr[sopt->rr+1];
 		soffset = sptr[sopt->rr+2];
 		dopt->rr = dopt->optlen + sizeof(struct iphdr);
 		memcpy(dptr, sptr+sopt->rr, optlen);
 		if (sopt->rr_needaddr && soffset <= optlen) {
 			if (soffset + 3 > optlen)
 				return -EINVAL;
 			dptr[2] = soffset + 4;
 			dopt->rr_needaddr = 1;
 		}
 		dptr += optlen;
 		dopt->optlen += optlen;
 	}
 	if (sopt->ts) {
 		optlen = sptr[sopt->ts+1];
 		soffset = sptr[sopt->ts+2];
 		dopt->ts = dopt->optlen + sizeof(struct iphdr);
 		memcpy(dptr, sptr+sopt->ts, optlen);
 		if (soffset <= optlen) {
 			if (sopt->ts_needaddr) {
 				if (soffset + 3 > optlen)
 					return -EINVAL;
 				dopt->ts_needaddr = 1;
 				soffset += 4;
 			}
 			if (sopt->ts_needtime) {
 				if (soffset + 3 > optlen)
 					return -EINVAL;
 				if ((dptr[3]&0xF) != IPOPT_TS_PRESPEC) {
 					dopt->ts_needtime = 1;
 					soffset += 4;
 				} else {
 					dopt->ts_needtime = 0;
 
 					if (soffset + 7 <= optlen) {
 						__be32 addr;
 
 						memcpy(&addr, dptr+soffset-1, 4);
 						if (inet_addr_type(dev_net(skb_dst(skb)->dev), addr) != RTN_UNICAST) {
 							dopt->ts_needtime = 1;
 							soffset += 8;
 						}
 					}
 				}
 			}
 			dptr[2] = soffset;
 		}
 		dptr += optlen;
  		dopt->optlen += optlen;
  	}
  	if (sopt->srr) {
		unsigned char * start = sptr+sopt->srr;
// 		unsigned char *start = sptr+sopt->srr;
  		__be32 faddr;
  
  		optlen  = start[1];
 		soffset = start[2];
 		doffset = 0;
 		if (soffset > optlen)
 			soffset = optlen + 1;
 		soffset -= 4;
 		if (soffset > 3) {
 			memcpy(&faddr, &start[soffset-1], 4);
 			for (soffset-=4, doffset=4; soffset > 3; soffset-=4, doffset+=4)
 				memcpy(&dptr[doffset-1], &start[soffset-1], 4);
 			 
 			if (memcmp(&ip_hdr(skb)->saddr,
 				   &start[soffset + 3], 4) == 0)
 				doffset -= 4;
 		}
 		if (doffset > 3) {
 			memcpy(&start[doffset-1], &daddr, 4);
 			dopt->faddr = faddr;
 			dptr[0] = start[0];
 			dptr[1] = doffset+3;
 			dptr[2] = 4;
 			dptr += doffset+3;
 			dopt->srr = dopt->optlen + sizeof(struct iphdr);
 			dopt->optlen += doffset+3;
 			dopt->is_strictroute = sopt->is_strictroute;
 		}
 	}
 	if (sopt->cipso) {
 		optlen  = sptr[sopt->cipso+1];
 		dopt->cipso = dopt->optlen+sizeof(struct iphdr);
 		memcpy(dptr, sptr+sopt->cipso, optlen);
 		dptr += optlen;
 		dopt->optlen += optlen;
 	}
 	while (dopt->optlen & 3) {
 		*dptr++ = IPOPT_END;
 		dopt->optlen++;
 	}
 	return 0;
 }