ieee80211_tx_h_dynamic_ps(struct ieee80211_tx_data *tx)
{
	struct ieee80211_local *local = tx->local;
	struct ieee80211_if_managed *ifmgd;

	 
	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS))
		return TX_CONTINUE;

	 
	if (local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS)
		return TX_CONTINUE;

	 
	if (local->hw.conf.dynamic_ps_timeout <= 0)
		return TX_CONTINUE;

	 
	if (local->scanning)
		return TX_CONTINUE;

	if (!local->ps_sdata)
		return TX_CONTINUE;

	 
	if (local->quiescing)
		return TX_CONTINUE;

	 
	if (tx->sdata->vif.type != NL80211_IFTYPE_STATION)
		return TX_CONTINUE;

	ifmgd = &tx->sdata->u.mgd;

	 
	if ((ifmgd->flags & IEEE80211_STA_UAPSD_ENABLED) &&
	    (ifmgd->uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VO) &&
	    skb_get_queue_mapping(tx->skb) == IEEE80211_AC_VO)
		return TX_CONTINUE;

	if (local->hw.conf.flags & IEEE80211_CONF_PS) {
		ieee80211_stop_queues_by_reason(&local->hw,
						IEEE80211_MAX_QUEUE_MAP,
						IEEE80211_QUEUE_STOP_REASON_PS);
		ifmgd->flags &= ~IEEE80211_STA_NULLFUNC_ACKED;
		ieee80211_queue_work(&local->hw,
				     &local->dynamic_ps_disable_work);
	}

	 
	if (!ifmgd->associated)
		return TX_CONTINUE;

	mod_timer(&local->dynamic_ps_timer, jiffies +
		  msecs_to_jiffies(local->hw.conf.dynamic_ps_timeout));

	return TX_CONTINUE;
}
