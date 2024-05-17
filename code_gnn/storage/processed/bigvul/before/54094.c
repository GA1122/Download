static int inet_rtm_newaddr(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	struct net *net = sock_net(skb->sk);
	struct in_ifaddr *ifa;
	struct in_ifaddr *ifa_existing;
	__u32 valid_lft = INFINITY_LIFE_TIME;
	__u32 prefered_lft = INFINITY_LIFE_TIME;

	ASSERT_RTNL();

	ifa = rtm_to_ifaddr(net, nlh, &valid_lft, &prefered_lft);
	if (IS_ERR(ifa))
		return PTR_ERR(ifa);

	ifa_existing = find_matching_ifa(ifa);
	if (!ifa_existing) {
		 
		set_ifa_lifetime(ifa, valid_lft, prefered_lft);
		if (ifa->ifa_flags & IFA_F_MCAUTOJOIN) {
			int ret = ip_mc_config(net->ipv4.mc_autojoin_sk,
					       true, ifa);

			if (ret < 0) {
				inet_free_ifa(ifa);
				return ret;
			}
		}
		return __inet_insert_ifa(ifa, nlh, NETLINK_CB(skb).portid);
	} else {
		inet_free_ifa(ifa);

		if (nlh->nlmsg_flags & NLM_F_EXCL ||
		    !(nlh->nlmsg_flags & NLM_F_REPLACE))
			return -EEXIST;
		ifa = ifa_existing;
		set_ifa_lifetime(ifa, valid_lft, prefered_lft);
		cancel_delayed_work(&check_lifetime_work);
		queue_delayed_work(system_power_efficient_wq,
				&check_lifetime_work, 0);
		rtmsg_ifa(RTM_NEWADDR, ifa, nlh, NETLINK_CB(skb).portid);
	}
	return 0;
}