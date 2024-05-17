static bool ip6_tlvopt_unknown(struct sk_buff *skb, int optoff)
{
	switch ((skb_network_header(skb)[optoff] & 0xC0) >> 6) {
	case 0:  
		return true;

	case 1:  
		break;

	case 3:  
		 
		if (ipv6_addr_is_multicast(&ipv6_hdr(skb)->daddr))
			break;
	case 2:  
		icmpv6_param_prob(skb, ICMPV6_UNK_OPTION, optoff);
		return false;
	}

	kfree_skb(skb);
	return false;
}
