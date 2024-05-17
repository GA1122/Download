static int dcbnl_build_peer_app(struct net_device *netdev, struct sk_buff* skb,
				int app_nested_type, int app_info_type,
				int app_entry_type)
{
	struct dcb_peer_app_info info;
	struct dcb_app *table = NULL;
	const struct dcbnl_rtnl_ops *ops = netdev->dcbnl_ops;
	u16 app_count;
	int err;


	 
	err = ops->peer_getappinfo(netdev, &info, &app_count);
	if (!err && app_count) {
		table = kmalloc(sizeof(struct dcb_app) * app_count, GFP_KERNEL);
		if (!table)
			return -ENOMEM;

		err = ops->peer_getapptable(netdev, table);
	}

	if (!err) {
		u16 i;
		struct nlattr *app;

		 
		err = -EMSGSIZE;

		app = nla_nest_start(skb, app_nested_type);
		if (!app)
			goto nla_put_failure;

		if (app_info_type &&
		    nla_put(skb, app_info_type, sizeof(info), &info))
			goto nla_put_failure;

		for (i = 0; i < app_count; i++) {
			if (nla_put(skb, app_entry_type, sizeof(struct dcb_app),
				    &table[i]))
				goto nla_put_failure;
		}
		nla_nest_end(skb, app);
	}
	err = 0;

nla_put_failure:
	kfree(table);
	return err;
}