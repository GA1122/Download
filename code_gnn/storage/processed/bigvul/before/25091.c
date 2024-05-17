bool nf_nat_proto_in_range(const struct nf_conntrack_tuple *tuple,
			   enum nf_nat_manip_type maniptype,
			   const union nf_conntrack_man_proto *min,
			   const union nf_conntrack_man_proto *max)
{
	__be16 port;

	if (maniptype == IP_NAT_MANIP_SRC)
		port = tuple->src.u.all;
	else
		port = tuple->dst.u.all;

	return ntohs(port) >= ntohs(min->all) &&
	       ntohs(port) <= ntohs(max->all);
}
