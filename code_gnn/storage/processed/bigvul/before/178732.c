 static int ip_options_get_finish(struct net *net, struct ip_options **optp,
				 struct ip_options *opt, int optlen)
// static int ip_options_get_finish(struct net *net, struct ip_options_rcu **optp,
// 				 struct ip_options_rcu *opt, int optlen)
  {
  	while (optlen & 3)
		opt->__data[optlen++] = IPOPT_END;
	opt->optlen = optlen;
	if (optlen && ip_options_compile(net, opt, NULL)) {
// 		opt->opt.__data[optlen++] = IPOPT_END;
// 	opt->opt.optlen = optlen;
// 	if (optlen && ip_options_compile(net, &opt->opt, NULL)) {
  		kfree(opt);
  		return -EINVAL;
  	}
 	kfree(*optp);
 	*optp = opt;
  	return 0;
  }