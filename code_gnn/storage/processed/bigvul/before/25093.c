int nf_nat_proto_range_to_nlattr(struct sk_buff *skb,
				 const struct nf_nat_range *range)
{
	NLA_PUT_BE16(skb, CTA_PROTONAT_PORT_MIN, range->min.all);
	NLA_PUT_BE16(skb, CTA_PROTONAT_PORT_MAX, range->max.all);
	return 0;

nla_put_failure:
	return -1;
}
