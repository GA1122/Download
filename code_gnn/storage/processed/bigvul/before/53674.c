static bool ip6_parse_tlv(const struct tlvtype_proc *procs, struct sk_buff *skb)
{
	const struct tlvtype_proc *curr;
	const unsigned char *nh = skb_network_header(skb);
	int off = skb_network_header_len(skb);
	int len = (skb_transport_header(skb)[1] + 1) << 3;
	int padlen = 0;

	if (skb_transport_offset(skb) + len > skb_headlen(skb))
		goto bad;

	off += 2;
	len -= 2;

	while (len > 0) {
		int optlen = nh[off + 1] + 2;
		int i;

		switch (nh[off]) {
		case IPV6_TLV_PAD1:
			optlen = 1;
			padlen++;
			if (padlen > 7)
				goto bad;
			break;

		case IPV6_TLV_PADN:
			 
			padlen += optlen;
			if (padlen > 7)
				goto bad;
			 
			for (i = 2; i < optlen; i++) {
				if (nh[off + i] != 0)
					goto bad;
			}
			break;

		default:  
			if (optlen > len)
				goto bad;
			for (curr = procs; curr->type >= 0; curr++) {
				if (curr->type == nh[off]) {
					 
					if (curr->func(skb, off) == false)
						return false;
					break;
				}
			}
			if (curr->type < 0) {
				if (ip6_tlvopt_unknown(skb, off) == 0)
					return false;
			}
			padlen = 0;
			break;
		}
		off += optlen;
		len -= optlen;
	}

	if (len == 0)
		return true;
bad:
	kfree_skb(skb);
	return false;
}
