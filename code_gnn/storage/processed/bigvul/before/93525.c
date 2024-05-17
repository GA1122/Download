static int ip6mr_fib_lookup(struct net *net, struct flowi6 *flp6,
			    struct mr6_table **mrt)
{
	int err;
	struct ip6mr_result res;
	struct fib_lookup_arg arg = {
		.result = &res,
		.flags = FIB_LOOKUP_NOREF,
	};

	err = fib_rules_lookup(net->ipv6.mr6_rules_ops,
			       flowi6_to_flowi(flp6), 0, &arg);
	if (err < 0)
		return err;
	*mrt = res.mrt;
	return 0;
}
