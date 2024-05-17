void nl80211_send_remain_on_channel(struct cfg80211_registered_device *rdev,
				    struct net_device *netdev, u64 cookie,
				    struct ieee80211_channel *chan,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration, gfp_t gfp)
{
	nl80211_send_remain_on_chan_event(NL80211_CMD_REMAIN_ON_CHANNEL,
					  rdev, netdev, cookie, chan,
					  channel_type, duration, gfp);
}