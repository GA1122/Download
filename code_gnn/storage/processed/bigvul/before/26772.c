static int nl80211_set_tx_bitrate_mask(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct nlattr *tb[NL80211_TXRATE_MAX + 1];
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct cfg80211_bitrate_mask mask;
	int rem, i;
	struct net_device *dev = info->user_ptr[1];
	struct nlattr *tx_rates;
	struct ieee80211_supported_band *sband;

	if (info->attrs[NL80211_ATTR_TX_RATES] == NULL)
		return -EINVAL;

	if (!rdev->ops->set_bitrate_mask)
		return -EOPNOTSUPP;

	memset(&mask, 0, sizeof(mask));
	 
	for (i = 0; i < IEEE80211_NUM_BANDS; i++) {
		sband = rdev->wiphy.bands[i];
		mask.control[i].legacy =
			sband ? (1 << sband->n_bitrates) - 1 : 0;
	}

	 
	nla_for_each_nested(tx_rates, info->attrs[NL80211_ATTR_TX_RATES], rem)
	{
		enum ieee80211_band band = nla_type(tx_rates);
		if (band < 0 || band >= IEEE80211_NUM_BANDS)
			return -EINVAL;
		sband = rdev->wiphy.bands[band];
		if (sband == NULL)
			return -EINVAL;
		nla_parse(tb, NL80211_TXRATE_MAX, nla_data(tx_rates),
			  nla_len(tx_rates), nl80211_txattr_policy);
		if (tb[NL80211_TXRATE_LEGACY]) {
			mask.control[band].legacy = rateset_to_mask(
				sband,
				nla_data(tb[NL80211_TXRATE_LEGACY]),
				nla_len(tb[NL80211_TXRATE_LEGACY]));
			if (mask.control[band].legacy == 0)
				return -EINVAL;
		}
	}

	return rdev->ops->set_bitrate_mask(&rdev->wiphy, dev, NULL, &mask);
}