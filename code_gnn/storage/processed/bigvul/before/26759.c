static int nl80211_send_survey(struct sk_buff *msg, u32 pid, u32 seq,
				int flags, struct net_device *dev,
				struct survey_info *survey)
{
	void *hdr;
	struct nlattr *infoattr;

	 
	if (!survey->channel)
		return -EINVAL;

	hdr = nl80211hdr_put(msg, pid, seq, flags,
			     NL80211_CMD_NEW_SURVEY_RESULTS);
	if (!hdr)
		return -ENOMEM;

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);

	infoattr = nla_nest_start(msg, NL80211_ATTR_SURVEY_INFO);
	if (!infoattr)
		goto nla_put_failure;

	NLA_PUT_U32(msg, NL80211_SURVEY_INFO_FREQUENCY,
		    survey->channel->center_freq);
	if (survey->filled & SURVEY_INFO_NOISE_DBM)
		NLA_PUT_U8(msg, NL80211_SURVEY_INFO_NOISE,
			    survey->noise);
	if (survey->filled & SURVEY_INFO_IN_USE)
		NLA_PUT_FLAG(msg, NL80211_SURVEY_INFO_IN_USE);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME,
			    survey->channel_time);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_BUSY)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_BUSY,
			    survey->channel_time_busy);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_EXT_BUSY)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_EXT_BUSY,
			    survey->channel_time_ext_busy);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_RX)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_RX,
			    survey->channel_time_rx);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_TX)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_TX,
			    survey->channel_time_tx);

	nla_nest_end(msg, infoattr);

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}