parse_tlv_tnl_enc_lim(struct sk_buff *skb, __u8 * raw)
{
	struct ipv6hdr *ipv6h = (struct ipv6hdr *) raw;
	__u8 nexthdr = ipv6h->nexthdr;
	__u16 off = sizeof (*ipv6h);

	while (ipv6_ext_hdr(nexthdr) && nexthdr != NEXTHDR_NONE) {
		__u16 optlen = 0;
		struct ipv6_opt_hdr *hdr;
		if (raw + off + sizeof (*hdr) > skb->data &&
		    !pskb_may_pull(skb, raw - skb->data + off + sizeof (*hdr)))
			break;

		hdr = (struct ipv6_opt_hdr *) (raw + off);
		if (nexthdr == NEXTHDR_FRAGMENT) {
			struct frag_hdr *frag_hdr = (struct frag_hdr *) hdr;
			if (frag_hdr->frag_off)
				break;
			optlen = 8;
		} else if (nexthdr == NEXTHDR_AUTH) {
			optlen = (hdr->hdrlen + 2) << 2;
		} else {
			optlen = ipv6_optlen(hdr);
		}
		if (nexthdr == NEXTHDR_DEST) {
			__u16 i = off + 2;
			while (1) {
				struct ipv6_tlv_tnl_enc_lim *tel;

				 
				if (i + sizeof (*tel) > off + optlen)
					break;

				tel = (struct ipv6_tlv_tnl_enc_lim *) &raw[i];
				 
				if (tel->type == IPV6_TLV_TNL_ENCAP_LIMIT &&
				    tel->length == 1)
					return i;
				 
				if (tel->type)
					i += tel->length + 2;
				else
					i++;
			}
		}
		nexthdr = hdr->nexthdr;
		off += optlen;
	}
	return 0;
}