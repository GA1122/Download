static int nl80211_addset_beacon(struct sk_buff *skb, struct genl_info *info)
{
        int (*call)(struct wiphy *wiphy, struct net_device *dev,
		    struct beacon_parameters *info);
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct beacon_parameters params;
	int haveinfo = 0, err;

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_BEACON_TAIL]))
		return -EINVAL;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	memset(&params, 0, sizeof(params));

	switch (info->genlhdr->cmd) {
	case NL80211_CMD_NEW_BEACON:
		 
		if (!info->attrs[NL80211_ATTR_BEACON_INTERVAL] ||
		    !info->attrs[NL80211_ATTR_DTIM_PERIOD] ||
		    !info->attrs[NL80211_ATTR_BEACON_HEAD])
			return -EINVAL;

		params.interval =
			nla_get_u32(info->attrs[NL80211_ATTR_BEACON_INTERVAL]);
		params.dtim_period =
			nla_get_u32(info->attrs[NL80211_ATTR_DTIM_PERIOD]);

		err = cfg80211_validate_beacon_int(rdev, params.interval);
		if (err)
			return err;

		call = rdev->ops->add_beacon;
		break;
	case NL80211_CMD_SET_BEACON:
		call = rdev->ops->set_beacon;
		break;
	default:
		WARN_ON(1);
		return -EOPNOTSUPP;
	}

	if (!call)
		return -EOPNOTSUPP;

	if (info->attrs[NL80211_ATTR_BEACON_HEAD]) {
		params.head = nla_data(info->attrs[NL80211_ATTR_BEACON_HEAD]);
		params.head_len =
		    nla_len(info->attrs[NL80211_ATTR_BEACON_HEAD]);
		haveinfo = 1;
	}

	if (info->attrs[NL80211_ATTR_BEACON_TAIL]) {
		params.tail = nla_data(info->attrs[NL80211_ATTR_BEACON_TAIL]);
		params.tail_len =
		    nla_len(info->attrs[NL80211_ATTR_BEACON_TAIL]);
		haveinfo = 1;
	}

	if (!haveinfo)
		return -EINVAL;

	err = call(&rdev->wiphy, dev, &params);
	if (!err && params.interval)
		wdev->beacon_interval = params.interval;
	return err;
}