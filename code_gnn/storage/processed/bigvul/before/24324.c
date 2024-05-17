int ieee80211_if_change_type(struct ieee80211_sub_if_data *sdata,
			     enum nl80211_iftype type)
{
	int ret;

	ASSERT_RTNL();

	if (type == ieee80211_vif_type_p2p(&sdata->vif))
		return 0;

	 
	if (sdata->local->oper_channel->flags & IEEE80211_CHAN_NO_IBSS &&
	    type == NL80211_IFTYPE_ADHOC)
		return -EOPNOTSUPP;

	if (ieee80211_sdata_running(sdata)) {
		ret = ieee80211_runtime_change_iftype(sdata, type);
		if (ret)
			return ret;
	} else {
		 
		ieee80211_teardown_sdata(sdata->dev);
		ieee80211_setup_sdata(sdata, type);
	}

	 
	sdata->vif.bss_conf.basic_rates =
		ieee80211_mandatory_rates(sdata->local,
			sdata->local->hw.conf.channel->band);
	sdata->drop_unencrypted = 0;
	if (type == NL80211_IFTYPE_STATION)
		sdata->u.mgd.use_4addr = false;

	return 0;
}