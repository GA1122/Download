static int __must_check __sta_info_destroy_part1(struct sta_info *sta)
{
	struct ieee80211_local *local;
	struct ieee80211_sub_if_data *sdata;
	int ret;

	might_sleep();

	if (!sta)
		return -ENOENT;

	local = sta->local;
	sdata = sta->sdata;

	lockdep_assert_held(&local->sta_mtx);

	 
	set_sta_flag(sta, WLAN_STA_BLOCK_BA);
	ieee80211_sta_tear_down_BA_sessions(sta, AGG_STOP_DESTROY_STA);

	ret = sta_info_hash_del(local, sta);
	if (WARN_ON(ret))
		return ret;

	list_del_rcu(&sta->list);

	drv_sta_pre_rcu_remove(local, sta->sdata, sta);

	if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN &&
	    rcu_access_pointer(sdata->u.vlan.sta) == sta)
		RCU_INIT_POINTER(sdata->u.vlan.sta, NULL);

	return 0;
}
