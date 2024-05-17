static int inet6_set_link_af(struct net_device *dev, const struct nlattr *nla)
{
	int err = -EINVAL;
	struct inet6_dev *idev = __in6_dev_get(dev);
	struct nlattr *tb[IFLA_INET6_MAX + 1];

	if (!idev)
		return -EAFNOSUPPORT;

	if (nla_parse_nested(tb, IFLA_INET6_MAX, nla, NULL) < 0)
		BUG();

	if (tb[IFLA_INET6_TOKEN]) {
		err = inet6_set_iftoken(idev, nla_data(tb[IFLA_INET6_TOKEN]));
		if (err)
			return err;
	}

	if (tb[IFLA_INET6_ADDR_GEN_MODE]) {
		u8 mode = nla_get_u8(tb[IFLA_INET6_ADDR_GEN_MODE]);

		if (mode != IN6_ADDR_GEN_MODE_EUI64 &&
		    mode != IN6_ADDR_GEN_MODE_NONE)
			return -EINVAL;
		idev->addr_gen_mode = mode;
		err = 0;
	}

	return err;
}