void sta_info_recalc_tim(struct sta_info *sta)
{
	struct ieee80211_local *local = sta->local;
	struct ps_data *ps;
	bool indicate_tim = false;
	u8 ignore_for_tim = sta->sta.uapsd_queues;
	int ac;
	u16 id;

	if (sta->sdata->vif.type == NL80211_IFTYPE_AP ||
	    sta->sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
		if (WARN_ON_ONCE(!sta->sdata->bss))
			return;

		ps = &sta->sdata->bss->ps;
		id = sta->sta.aid;
#ifdef CONFIG_MAC80211_MESH
	} else if (ieee80211_vif_is_mesh(&sta->sdata->vif)) {
		ps = &sta->sdata->u.mesh.ps;
		 
		id = sta->plid % (IEEE80211_MAX_AID + 1);
#endif
	} else {
		return;
	}

	 
	if (local->hw.flags & IEEE80211_HW_AP_LINK_PS)
		return;

	if (sta->dead)
		goto done;

	 
	if (ignore_for_tim == BIT(IEEE80211_NUM_ACS) - 1)
		ignore_for_tim = 0;

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		unsigned long tids;

		if (ignore_for_tim & BIT(ac))
			continue;

		indicate_tim |= !skb_queue_empty(&sta->tx_filtered[ac]) ||
				!skb_queue_empty(&sta->ps_tx_buf[ac]);
		if (indicate_tim)
			break;

		tids = ieee80211_tids_for_ac(ac);

		indicate_tim |=
			sta->driver_buffered_tids & tids;
	}

 done:
	spin_lock_bh(&local->tim_lock);

	if (indicate_tim == __bss_tim_get(ps->tim, id))
		goto out_unlock;

	if (indicate_tim)
		__bss_tim_set(ps->tim, id);
	else
		__bss_tim_clear(ps->tim, id);

	if (local->ops->set_tim) {
		local->tim_in_locked_section = true;
		drv_set_tim(local, &sta->sta, indicate_tim);
		local->tim_in_locked_section = false;
	}

out_unlock:
	spin_unlock_bh(&local->tim_lock);
}
