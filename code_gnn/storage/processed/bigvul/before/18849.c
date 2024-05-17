int cipso_v4_skbuff_setattr(struct sk_buff *skb,
			    const struct cipso_v4_doi *doi_def,
			    const struct netlbl_lsm_secattr *secattr)
{
	int ret_val;
	struct iphdr *iph;
	struct ip_options *opt = &IPCB(skb)->opt;
	unsigned char buf[CIPSO_V4_OPT_LEN_MAX];
	u32 buf_len = CIPSO_V4_OPT_LEN_MAX;
	u32 opt_len;
	int len_delta;

	ret_val = cipso_v4_genopt(buf, buf_len, doi_def, secattr);
	if (ret_val < 0)
		return ret_val;
	buf_len = ret_val;
	opt_len = (buf_len + 3) & ~3;

	 

	len_delta = opt_len - opt->optlen;
	 
	ret_val = skb_cow(skb, skb_headroom(skb) + len_delta);
	if (ret_val < 0)
		return ret_val;

	if (len_delta > 0) {
		 
		iph = ip_hdr(skb);
		skb_push(skb, len_delta);
		memmove((char *)iph - len_delta, iph, iph->ihl << 2);
		skb_reset_network_header(skb);
		iph = ip_hdr(skb);
	} else if (len_delta < 0) {
		iph = ip_hdr(skb);
		memset(iph + 1, IPOPT_NOP, opt->optlen);
	} else
		iph = ip_hdr(skb);

	if (opt->optlen > 0)
		memset(opt, 0, sizeof(*opt));
	opt->optlen = opt_len;
	opt->cipso = sizeof(struct iphdr);
	opt->is_changed = 1;

	 
	memcpy(iph + 1, buf, buf_len);
	if (opt_len > buf_len)
		memset((char *)(iph + 1) + buf_len, 0, opt_len - buf_len);
	if (len_delta != 0) {
		iph->ihl = 5 + (opt_len >> 2);
		iph->tot_len = htons(skb->len);
	}
	ip_send_check(iph);

	return 0;
}
