nl80211_send_cqm_rssi_notify(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev,
			     enum nl80211_cqm_rssi_threshold_event rssi_event,
			     gfp_t gfp)
{
	struct sk_buff *msg;
	struct nlattr *pinfoattr;
	void *hdr;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_NOTIFY_CQM);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);

	pinfoattr = nla_nest_start(msg, NL80211_ATTR_CQM);
	if (!pinfoattr)
		goto nla_put_failure;

	NLA_PUT_U32(msg, NL80211_ATTR_CQM_RSSI_THRESHOLD_EVENT,
		    rssi_event);

	nla_nest_end(msg, pinfoattr);

	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}