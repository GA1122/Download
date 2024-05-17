icmp_match(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct icmphdr *ic;
	struct icmphdr _icmph;
	const struct ipt_icmp *icmpinfo = par->matchinfo;

	 
	if (par->fragoff != 0)
		return false;

	ic = skb_header_pointer(skb, par->thoff, sizeof(_icmph), &_icmph);
	if (ic == NULL) {
		 
		duprintf("Dropping evil ICMP tinygram.\n");
		par->hotdrop = true;
		return false;
	}

	return icmp_type_code_match(icmpinfo->type,
				    icmpinfo->code[0],
				    icmpinfo->code[1],
				    ic->type, ic->code,
				    !!(icmpinfo->invflags&IPT_ICMP_INV));
}
