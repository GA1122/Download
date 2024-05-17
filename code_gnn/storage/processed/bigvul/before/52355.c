static int icmp6_checkentry(const struct xt_mtchk_param *par)
{
	const struct ip6t_icmp *icmpinfo = par->matchinfo;

	 
	return (icmpinfo->invflags & ~IP6T_ICMP_INV) ? -EINVAL : 0;
}
