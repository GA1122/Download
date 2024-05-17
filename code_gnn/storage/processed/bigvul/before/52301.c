static int icmp_checkentry(const struct xt_mtchk_param *par)
{
	const struct ipt_icmp *icmpinfo = par->matchinfo;

	 
	return (icmpinfo->invflags & ~IPT_ICMP_INV) ? -EINVAL : 0;
}