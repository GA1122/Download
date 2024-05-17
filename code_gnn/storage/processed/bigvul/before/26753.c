void nl80211_send_sched_scan(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, u32 cmd)
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!msg)
		return;

	if (nl80211_send_sched_scan_msg(msg, rdev, netdev, 0, 0, 0, cmd) < 0) {
		nlmsg_free(msg);
		return;
	}

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_scan_mcgrp.id, GFP_KERNEL);
}