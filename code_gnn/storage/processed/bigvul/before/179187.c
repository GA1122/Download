 static void ip6_append_data_mtu(int *mtu,
// static void ip6_append_data_mtu(unsigned int *mtu,
  				int *maxfraglen,
  				unsigned int fragheaderlen,
  				struct sk_buff *skb,
				struct rt6_info *rt)
// 				struct rt6_info *rt,
// 				bool pmtuprobe)
  {
  	if (!(rt->dst.flags & DST_XFRM_TUNNEL)) {
  		if (skb == NULL) {
 			 
 			*mtu = *mtu - rt->dst.header_len;
 
 		} else {
 			 
			*mtu = dst_mtu(rt->dst.path);
// 			*mtu = min(*mtu, pmtuprobe ?
// 				   rt->dst.dev->mtu :
// 				   dst_mtu(rt->dst.path));
  		}
  		*maxfraglen = ((*mtu - fragheaderlen) & ~7)
  			      + fragheaderlen - sizeof(struct frag_hdr);
 	}
 }