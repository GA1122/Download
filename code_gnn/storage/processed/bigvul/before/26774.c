static int nl80211_set_wiphy(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev;
	struct net_device *netdev = NULL;
	struct wireless_dev *wdev;
	int result = 0, rem_txq_params = 0;
	struct nlattr *nl_txq_params;
	u32 changed;
	u8 retry_short = 0, retry_long = 0;
	u32 frag_threshold = 0, rts_threshold = 0;
	u8 coverage_class = 0;

	 
	mutex_lock(&cfg80211_mutex);

	if (info->attrs[NL80211_ATTR_IFINDEX]) {
		int ifindex = nla_get_u32(info->attrs[NL80211_ATTR_IFINDEX]);

		netdev = dev_get_by_index(genl_info_net(info), ifindex);
		if (netdev && netdev->ieee80211_ptr) {
			rdev = wiphy_to_dev(netdev->ieee80211_ptr->wiphy);
			mutex_lock(&rdev->mtx);
		} else
			netdev = NULL;
	}

	if (!netdev) {
		rdev = __cfg80211_rdev_from_info(info);
		if (IS_ERR(rdev)) {
			mutex_unlock(&cfg80211_mutex);
			return PTR_ERR(rdev);
		}
		wdev = NULL;
		netdev = NULL;
		result = 0;

		mutex_lock(&rdev->mtx);
	} else if (netif_running(netdev) &&
		   nl80211_can_set_dev_channel(netdev->ieee80211_ptr))
		wdev = netdev->ieee80211_ptr;
	else
		wdev = NULL;

	 

	if (info->attrs[NL80211_ATTR_WIPHY_NAME])
		result = cfg80211_dev_rename(
			rdev, nla_data(info->attrs[NL80211_ATTR_WIPHY_NAME]));

	mutex_unlock(&cfg80211_mutex);

	if (result)
		goto bad_res;

	if (info->attrs[NL80211_ATTR_WIPHY_TXQ_PARAMS]) {
		struct ieee80211_txq_params txq_params;
		struct nlattr *tb[NL80211_TXQ_ATTR_MAX + 1];

		if (!rdev->ops->set_txq_params) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		nla_for_each_nested(nl_txq_params,
				    info->attrs[NL80211_ATTR_WIPHY_TXQ_PARAMS],
				    rem_txq_params) {
			nla_parse(tb, NL80211_TXQ_ATTR_MAX,
				  nla_data(nl_txq_params),
				  nla_len(nl_txq_params),
				  txq_params_policy);
			result = parse_txq_params(tb, &txq_params);
			if (result)
				goto bad_res;

			result = rdev->ops->set_txq_params(&rdev->wiphy,
							   &txq_params);
			if (result)
				goto bad_res;
		}
	}

	if (info->attrs[NL80211_ATTR_WIPHY_FREQ]) {
		result = __nl80211_set_channel(rdev, wdev, info);
		if (result)
			goto bad_res;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_TX_POWER_SETTING]) {
		enum nl80211_tx_power_setting type;
		int idx, mbm = 0;

		if (!rdev->ops->set_tx_power) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		idx = NL80211_ATTR_WIPHY_TX_POWER_SETTING;
		type = nla_get_u32(info->attrs[idx]);

		if (!info->attrs[NL80211_ATTR_WIPHY_TX_POWER_LEVEL] &&
		    (type != NL80211_TX_POWER_AUTOMATIC)) {
			result = -EINVAL;
			goto bad_res;
		}

		if (type != NL80211_TX_POWER_AUTOMATIC) {
			idx = NL80211_ATTR_WIPHY_TX_POWER_LEVEL;
			mbm = nla_get_u32(info->attrs[idx]);
		}

		result = rdev->ops->set_tx_power(&rdev->wiphy, type, mbm);
		if (result)
			goto bad_res;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_ANTENNA_TX] &&
	    info->attrs[NL80211_ATTR_WIPHY_ANTENNA_RX]) {
		u32 tx_ant, rx_ant;
		if ((!rdev->wiphy.available_antennas_tx &&
		     !rdev->wiphy.available_antennas_rx) ||
		    !rdev->ops->set_antenna) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		tx_ant = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_ANTENNA_TX]);
		rx_ant = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_ANTENNA_RX]);

		 
		if ((~tx_ant && (tx_ant & ~rdev->wiphy.available_antennas_tx)) ||
		    (~rx_ant && (rx_ant & ~rdev->wiphy.available_antennas_rx))) {
			result = -EINVAL;
			goto bad_res;
		}

		tx_ant = tx_ant & rdev->wiphy.available_antennas_tx;
		rx_ant = rx_ant & rdev->wiphy.available_antennas_rx;

		result = rdev->ops->set_antenna(&rdev->wiphy, tx_ant, rx_ant);
		if (result)
			goto bad_res;
	}

	changed = 0;

	if (info->attrs[NL80211_ATTR_WIPHY_RETRY_SHORT]) {
		retry_short = nla_get_u8(
			info->attrs[NL80211_ATTR_WIPHY_RETRY_SHORT]);
		if (retry_short == 0) {
			result = -EINVAL;
			goto bad_res;
		}
		changed |= WIPHY_PARAM_RETRY_SHORT;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_RETRY_LONG]) {
		retry_long = nla_get_u8(
			info->attrs[NL80211_ATTR_WIPHY_RETRY_LONG]);
		if (retry_long == 0) {
			result = -EINVAL;
			goto bad_res;
		}
		changed |= WIPHY_PARAM_RETRY_LONG;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_FRAG_THRESHOLD]) {
		frag_threshold = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_FRAG_THRESHOLD]);
		if (frag_threshold < 256) {
			result = -EINVAL;
			goto bad_res;
		}
		if (frag_threshold != (u32) -1) {
			 
			frag_threshold &= ~0x1;
		}
		changed |= WIPHY_PARAM_FRAG_THRESHOLD;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_RTS_THRESHOLD]) {
		rts_threshold = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_RTS_THRESHOLD]);
		changed |= WIPHY_PARAM_RTS_THRESHOLD;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_COVERAGE_CLASS]) {
		coverage_class = nla_get_u8(
			info->attrs[NL80211_ATTR_WIPHY_COVERAGE_CLASS]);
		changed |= WIPHY_PARAM_COVERAGE_CLASS;
	}

	if (changed) {
		u8 old_retry_short, old_retry_long;
		u32 old_frag_threshold, old_rts_threshold;
		u8 old_coverage_class;

		if (!rdev->ops->set_wiphy_params) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		old_retry_short = rdev->wiphy.retry_short;
		old_retry_long = rdev->wiphy.retry_long;
		old_frag_threshold = rdev->wiphy.frag_threshold;
		old_rts_threshold = rdev->wiphy.rts_threshold;
		old_coverage_class = rdev->wiphy.coverage_class;

		if (changed & WIPHY_PARAM_RETRY_SHORT)
			rdev->wiphy.retry_short = retry_short;
		if (changed & WIPHY_PARAM_RETRY_LONG)
			rdev->wiphy.retry_long = retry_long;
		if (changed & WIPHY_PARAM_FRAG_THRESHOLD)
			rdev->wiphy.frag_threshold = frag_threshold;
		if (changed & WIPHY_PARAM_RTS_THRESHOLD)
			rdev->wiphy.rts_threshold = rts_threshold;
		if (changed & WIPHY_PARAM_COVERAGE_CLASS)
			rdev->wiphy.coverage_class = coverage_class;

		result = rdev->ops->set_wiphy_params(&rdev->wiphy, changed);
		if (result) {
			rdev->wiphy.retry_short = old_retry_short;
			rdev->wiphy.retry_long = old_retry_long;
			rdev->wiphy.frag_threshold = old_frag_threshold;
			rdev->wiphy.rts_threshold = old_rts_threshold;
			rdev->wiphy.coverage_class = old_coverage_class;
		}
	}

 bad_res:
	mutex_unlock(&rdev->mtx);
	if (netdev)
		dev_put(netdev);
	return result;
}