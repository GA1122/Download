static int ieee80211_runtime_change_iftype(struct ieee80211_sub_if_data *sdata,
					   enum nl80211_iftype type)
{
	struct ieee80211_local *local = sdata->local;
	int ret, err;
	enum nl80211_iftype internal_type = type;
	bool p2p = false;

	ASSERT_RTNL();

	if (!local->ops->change_interface)
		return -EBUSY;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
		 
		break;
	default:
		return -EBUSY;
	}

	switch (type) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
		 
		break;
	case NL80211_IFTYPE_P2P_CLIENT:
		p2p = true;
		internal_type = NL80211_IFTYPE_STATION;
		break;
	case NL80211_IFTYPE_P2P_GO:
		p2p = true;
		internal_type = NL80211_IFTYPE_AP;
		break;
	default:
		return -EBUSY;
	}

	ret = ieee80211_check_concurrent_iface(sdata, internal_type);
	if (ret)
		return ret;

	ieee80211_do_stop(sdata, false);

	ieee80211_teardown_sdata(sdata->dev);

	ret = drv_change_interface(local, sdata, internal_type, p2p);
	if (ret)
		type = sdata->vif.type;

	ieee80211_setup_sdata(sdata, type);

	err = ieee80211_do_open(sdata->dev, false);
	WARN(err, "type change: do_open returned %d", err);

	return ret;
}