struct ipv6_txoptions *ipv6_fixup_options(struct ipv6_txoptions *opt_space,
					  struct ipv6_txoptions *opt)
{
	 
	if (opt && opt->dst0opt && !opt->srcrt) {
		if (opt_space != opt) {
			memcpy(opt_space, opt, sizeof(*opt_space));
			opt = opt_space;
		}
		opt->opt_nflen -= ipv6_optlen(opt->dst0opt);
		opt->dst0opt = NULL;
	}

	return opt;
}