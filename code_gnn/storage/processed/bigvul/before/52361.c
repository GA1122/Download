static int __init ip6_tables_init(void)
{
	int ret;

	ret = register_pernet_subsys(&ip6_tables_net_ops);
	if (ret < 0)
		goto err1;

	 
	ret = xt_register_targets(ip6t_builtin_tg, ARRAY_SIZE(ip6t_builtin_tg));
	if (ret < 0)
		goto err2;
	ret = xt_register_matches(ip6t_builtin_mt, ARRAY_SIZE(ip6t_builtin_mt));
	if (ret < 0)
		goto err4;

	 
	ret = nf_register_sockopt(&ip6t_sockopts);
	if (ret < 0)
		goto err5;

	pr_info("(C) 2000-2006 Netfilter Core Team\n");
	return 0;

err5:
	xt_unregister_matches(ip6t_builtin_mt, ARRAY_SIZE(ip6t_builtin_mt));
err4:
	xt_unregister_targets(ip6t_builtin_tg, ARRAY_SIZE(ip6t_builtin_tg));
err2:
	unregister_pernet_subsys(&ip6_tables_net_ops);
err1:
	return ret;
}