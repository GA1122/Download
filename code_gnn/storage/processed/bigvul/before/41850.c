static void inet6_prefix_notify(int event, struct inet6_dev *idev,
			 struct prefix_info *pinfo)
{
	struct sk_buff *skb;
	struct net *net = dev_net(idev->dev);
	int err = -ENOBUFS;

	skb = nlmsg_new(inet6_prefix_nlmsg_size(), GFP_ATOMIC);
	if (skb == NULL)
		goto errout;

	err = inet6_fill_prefix(skb, idev, pinfo, 0, 0, event, 0);
	if (err < 0) {
		 
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_PREFIX, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_IPV6_PREFIX, err);
}