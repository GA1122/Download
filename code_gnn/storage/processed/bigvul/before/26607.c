int __ip6_local_out(struct sk_buff *skb)
{
	int len;

	len = skb->len - sizeof(struct ipv6hdr);
	if (len > IPV6_MAXPLEN)
		len = 0;
	ipv6_hdr(skb)->payload_len = htons(len);

	return nf_hook(NFPROTO_IPV6, NF_INET_LOCAL_OUT, skb, NULL,
		       skb_dst(skb)->dev, dst_output);
}