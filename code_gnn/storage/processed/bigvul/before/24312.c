u32 __ieee80211_recalc_idle(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;
	int count = 0;
	bool working = false, scanning = false, hw_roc = false;
	struct ieee80211_work *wk;
	unsigned int led_trig_start = 0, led_trig_stop = 0;

#ifdef CONFIG_PROVE_LOCKING
	WARN_ON(debug_locks && !lockdep_rtnl_is_held() &&
		!lockdep_is_held(&local->iflist_mtx));
#endif
	lockdep_assert_held(&local->mtx);

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata)) {
			sdata->vif.bss_conf.idle = true;
			continue;
		}

		sdata->old_idle = sdata->vif.bss_conf.idle;

		 
		if (sdata->vif.type == NL80211_IFTYPE_STATION &&
		    !sdata->u.mgd.associated) {
			sdata->vif.bss_conf.idle = true;
			continue;
		}
		 
		if (sdata->vif.type == NL80211_IFTYPE_ADHOC &&
		    !sdata->u.ibss.ssid_len) {
			sdata->vif.bss_conf.idle = true;
			continue;
		}
		 
		count++;
	}

	list_for_each_entry(wk, &local->work_list, list) {
		working = true;
		wk->sdata->vif.bss_conf.idle = false;
	}

	if (local->scan_sdata) {
		scanning = true;
		local->scan_sdata->vif.bss_conf.idle = false;
	}

	if (local->hw_roc_channel)
		hw_roc = true;

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->old_idle == sdata->vif.bss_conf.idle)
			continue;
		if (!ieee80211_sdata_running(sdata))
			continue;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_IDLE);
	}

	if (working || scanning || hw_roc)
		led_trig_start |= IEEE80211_TPT_LEDTRIG_FL_WORK;
	else
		led_trig_stop |= IEEE80211_TPT_LEDTRIG_FL_WORK;

	if (count)
		led_trig_start |= IEEE80211_TPT_LEDTRIG_FL_CONNECTED;
	else
		led_trig_stop |= IEEE80211_TPT_LEDTRIG_FL_CONNECTED;

	ieee80211_mod_tpt_led_trig(local, led_trig_start, led_trig_stop);

	if (hw_roc)
		return ieee80211_idle_off(local, "hw remain-on-channel");
	if (working)
		return ieee80211_idle_off(local, "working");
	if (scanning)
		return ieee80211_idle_off(local, "scanning");
	if (!count)
		return ieee80211_idle_on(local);
	else
		return ieee80211_idle_off(local, "in use");

	return 0;
}