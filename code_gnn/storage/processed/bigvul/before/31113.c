static int dcbnl_setapp(struct net_device *netdev, struct nlmsghdr *nlh,
			u32 seq, struct nlattr **tb, struct sk_buff *skb)
{
	int ret;
	u16 id;
	u8 up, idtype;
	struct nlattr *app_tb[DCB_APP_ATTR_MAX + 1];

	if (!tb[DCB_ATTR_APP])
		return -EINVAL;

	ret = nla_parse_nested(app_tb, DCB_APP_ATTR_MAX, tb[DCB_ATTR_APP],
	                       dcbnl_app_nest);
	if (ret)
		return ret;

	 
	if ((!app_tb[DCB_APP_ATTR_IDTYPE]) ||
	    (!app_tb[DCB_APP_ATTR_ID]) ||
	    (!app_tb[DCB_APP_ATTR_PRIORITY]))
		return -EINVAL;

	 
	idtype = nla_get_u8(app_tb[DCB_APP_ATTR_IDTYPE]);
	if ((idtype != DCB_APP_IDTYPE_ETHTYPE) &&
	    (idtype != DCB_APP_IDTYPE_PORTNUM))
		return -EINVAL;

	id = nla_get_u16(app_tb[DCB_APP_ATTR_ID]);
	up = nla_get_u8(app_tb[DCB_APP_ATTR_PRIORITY]);

	if (netdev->dcbnl_ops->setapp) {
		ret = netdev->dcbnl_ops->setapp(netdev, idtype, id, up);
	} else {
		struct dcb_app app;
		app.selector = idtype;
		app.protocol = id;
		app.priority = up;
		ret = dcb_setapp(netdev, &app);
	}

	ret = nla_put_u8(skb, DCB_ATTR_APP, ret);
	dcbnl_cee_notify(netdev, RTM_SETDCB, DCB_CMD_SAPP, seq, 0);

	return ret;
}