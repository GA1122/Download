static int nl80211_remain_on_channel(struct sk_buff *skb,
				     struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct ieee80211_channel *chan;
	struct sk_buff *msg;
	void *hdr;
	u64 cookie;
	enum nl80211_channel_type channel_type = NL80211_CHAN_NO_HT;
	u32 freq, duration;
	int err;

	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ] ||
	    !info->attrs[NL80211_ATTR_DURATION])
		return -EINVAL;

	duration = nla_get_u32(info->attrs[NL80211_ATTR_DURATION]);

	 
	if (!duration || !msecs_to_jiffies(duration) ||
	    duration > rdev->wiphy.max_remain_on_channel_duration)
		return -EINVAL;

	if (!rdev->ops->remain_on_channel)
		return -EOPNOTSUPP;

	if (info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]) {
		channel_type = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]);
		if (channel_type != NL80211_CHAN_NO_HT &&
		    channel_type != NL80211_CHAN_HT20 &&
		    channel_type != NL80211_CHAN_HT40PLUS &&
		    channel_type != NL80211_CHAN_HT40MINUS)
			return -EINVAL;
	}

	freq = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]);
	chan = rdev_freq_to_chan(rdev, freq, channel_type);
	if (chan == NULL)
		return -EINVAL;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
			     NL80211_CMD_REMAIN_ON_CHANNEL);

	if (IS_ERR(hdr)) {
		err = PTR_ERR(hdr);
		goto free_msg;
	}

	err = rdev->ops->remain_on_channel(&rdev->wiphy, dev, chan,
					   channel_type, duration, &cookie);

	if (err)
		goto free_msg;

	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);

	genlmsg_end(msg, hdr);

	return genlmsg_reply(msg, info);

 nla_put_failure:
	err = -ENOBUFS;
 free_msg:
	nlmsg_free(msg);
	return err;
}