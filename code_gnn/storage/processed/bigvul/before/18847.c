int cipso_v4_skbuff_delattr(struct sk_buff *skb)
{
	int ret_val;
	struct iphdr *iph;
	struct ip_options *opt = &IPCB(skb)->opt;
	unsigned char *cipso_ptr;

	if (opt->cipso == 0)
		return 0;

	 
	ret_val = skb_cow(skb, skb_headroom(skb));
	if (ret_val < 0)
		return ret_val;

	 

	iph = ip_hdr(skb);
	cipso_ptr = (unsigned char *)iph + opt->cipso;
	memset(cipso_ptr, IPOPT_NOOP, cipso_ptr[1]);
	opt->cipso = 0;
	opt->is_changed = 1;

	ip_send_check(iph);

	return 0;
}
