static int dcbnl_getapp(struct net_device *netdev, struct nlmsghdr *nlh,
			u32 seq, struct nlattr **tb, struct sk_buff *skb)
{
	struct nlattr *app_nest;
	struct nlattr *app_tb[DCB_APP_ATTR_MAX + 1];
	u16 id;
	u8 up, idtype;
	int ret;

	if (!tb[DCB_ATTR_APP])
		return -EINVAL;

	ret = nla_parse_nested(app_tb, DCB_APP_ATTR_MAX, tb[DCB_ATTR_APP],
	                       dcbnl_app_nest);
	if (ret)
		return ret;

	 
	if ((!app_tb[DCB_APP_ATTR_IDTYPE]) ||
	    (!app_tb[DCB_APP_ATTR_ID]))
		return -EINVAL;

	 
	idtype = nla_get_u8(app_tb[DCB_APP_ATTR_IDTYPE]);
	if ((idtype != DCB_APP_IDTYPE_ETHTYPE) &&
	    (idtype != DCB_APP_IDTYPE_PORTNUM))
		return -EINVAL;

	id = nla_get_u16(app_tb[DCB_APP_ATTR_ID]);

	if (netdev->dcbnl_ops->getapp) {
		up = netdev->dcbnl_ops->getapp(netdev, idtype, id);
	} else {
		struct dcb_app app = {
					.selector = idtype,
					.protocol = id,
				     };
		up = dcb_getapp(netdev, &app);
	}

	app_nest = nla_nest_start(skb, DCB_ATTR_APP);
	if (!app_nest)
		return -EMSGSIZE;

	ret = nla_put_u8(skb, DCB_APP_ATTR_IDTYPE, idtype);
	if (ret)
		goto out_cancel;

	ret = nla_put_u16(skb, DCB_APP_ATTR_ID, id);
	if (ret)
		goto out_cancel;

	ret = nla_put_u8(skb, DCB_APP_ATTR_PRIORITY, up);
	if (ret)
		goto out_cancel;

	nla_nest_end(skb, app_nest);

	return 0;

out_cancel:
	nla_nest_cancel(skb, app_nest);
	return ret;
}