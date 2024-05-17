static int dccp_nlattr_size(void)
{
	return nla_total_size(0)	 
		+ nla_policy_len(dccp_nla_policy, CTA_PROTOINFO_DCCP_MAX + 1);
}
