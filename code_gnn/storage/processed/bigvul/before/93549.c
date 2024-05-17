static int __net_init ip6mr_rules_init(struct net *net)
{
	struct fib_rules_ops *ops;
	struct mr6_table *mrt;
	int err;

	ops = fib_rules_register(&ip6mr_rules_ops_template, net);
	if (IS_ERR(ops))
		return PTR_ERR(ops);

	INIT_LIST_HEAD(&net->ipv6.mr6_tables);

	mrt = ip6mr_new_table(net, RT6_TABLE_DFLT);
	if (!mrt) {
		err = -ENOMEM;
		goto err1;
	}

	err = fib_default_rule_add(ops, 0x7fff, RT6_TABLE_DFLT, 0);
	if (err < 0)
		goto err2;

	net->ipv6.mr6_rules_ops = ops;
	return 0;

err2:
	ip6mr_free_table(mrt);
err1:
	fib_rules_unregister(ops);
	return err;
}
