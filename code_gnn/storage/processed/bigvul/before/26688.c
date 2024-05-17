static int nl80211_get_mesh_config(struct sk_buff *skb,
				   struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct mesh_config cur_params;
	int err = 0;
	void *hdr;
	struct nlattr *pinfoattr;
	struct sk_buff *msg;

	if (wdev->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

	if (!rdev->ops->get_mesh_config)
		return -EOPNOTSUPP;

	wdev_lock(wdev);
	 
	if (!wdev->mesh_id_len)
		memcpy(&cur_params, &default_mesh_config, sizeof(cur_params));
	else
		err = rdev->ops->get_mesh_config(&rdev->wiphy, dev,
						 &cur_params);
	wdev_unlock(wdev);

	if (err)
		return err;

	 
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;
	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
			     NL80211_CMD_GET_MESH_CONFIG);
	if (!hdr)
		goto out;
	pinfoattr = nla_nest_start(msg, NL80211_ATTR_MESH_CONFIG);
	if (!pinfoattr)
		goto nla_put_failure;
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT_U16(msg, NL80211_MESHCONF_RETRY_TIMEOUT,
			cur_params.dot11MeshRetryTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_CONFIRM_TIMEOUT,
			cur_params.dot11MeshConfirmTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_HOLDING_TIMEOUT,
			cur_params.dot11MeshHoldingTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_MAX_PEER_LINKS,
			cur_params.dot11MeshMaxPeerLinks);
	NLA_PUT_U8(msg, NL80211_MESHCONF_MAX_RETRIES,
			cur_params.dot11MeshMaxRetries);
	NLA_PUT_U8(msg, NL80211_MESHCONF_TTL,
			cur_params.dot11MeshTTL);
	NLA_PUT_U8(msg, NL80211_MESHCONF_ELEMENT_TTL,
			cur_params.element_ttl);
	NLA_PUT_U8(msg, NL80211_MESHCONF_AUTO_OPEN_PLINKS,
			cur_params.auto_open_plinks);
	NLA_PUT_U8(msg, NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES,
			cur_params.dot11MeshHWMPmaxPREQretries);
	NLA_PUT_U32(msg, NL80211_MESHCONF_PATH_REFRESH_TIME,
			cur_params.path_refresh_time);
	NLA_PUT_U16(msg, NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT,
			cur_params.min_discovery_timeout);
	NLA_PUT_U32(msg, NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT,
			cur_params.dot11MeshHWMPactivePathTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL,
			cur_params.dot11MeshHWMPpreqMinInterval);
	NLA_PUT_U16(msg, NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
			cur_params.dot11MeshHWMPnetDiameterTraversalTime);
	NLA_PUT_U8(msg, NL80211_MESHCONF_HWMP_ROOTMODE,
			cur_params.dot11MeshHWMPRootMode);
	nla_nest_end(msg, pinfoattr);
	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
 out:
	nlmsg_free(msg);
	return -ENOBUFS;
}