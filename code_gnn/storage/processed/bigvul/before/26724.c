static int nl80211_req_set_reg(struct sk_buff *skb, struct genl_info *info)
{
	int r;
	char *data = NULL;

	 
	mutex_lock(&cfg80211_mutex);
	if (unlikely(!cfg80211_regdomain)) {
		mutex_unlock(&cfg80211_mutex);
		return -EINPROGRESS;
	}
	mutex_unlock(&cfg80211_mutex);

	if (!info->attrs[NL80211_ATTR_REG_ALPHA2])
		return -EINVAL;

	data = nla_data(info->attrs[NL80211_ATTR_REG_ALPHA2]);

	r = regulatory_hint_user(data);

	return r;
}
