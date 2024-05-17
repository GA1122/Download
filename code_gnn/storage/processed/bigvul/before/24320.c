static void ieee80211_do_stop(struct ieee80211_sub_if_data *sdata,
			      bool going_down)
{
	struct ieee80211_local *local = sdata->local;
	unsigned long flags;
	struct sk_buff *skb, *tmp;
	u32 hw_reconf_flags = 0;
	int i;
	enum nl80211_channel_type orig_ct;

	clear_bit(SDATA_STATE_RUNNING, &sdata->state);

	if (local->scan_sdata == sdata)
		ieee80211_scan_cancel(local);

	 
	netif_tx_stop_all_queues(sdata->dev);

	 
	ieee80211_work_purge(sdata);

	 
	sta_info_flush(local, sdata);

	 
	if (sdata->flags & IEEE80211_SDATA_ALLMULTI)
		atomic_dec(&local->iff_allmultis);

	if (sdata->flags & IEEE80211_SDATA_PROMISC)
		atomic_dec(&local->iff_promiscs);

	if (sdata->vif.type == NL80211_IFTYPE_AP) {
		local->fif_pspoll--;
		local->fif_probe_req--;
	} else if (sdata->vif.type == NL80211_IFTYPE_ADHOC) {
		local->fif_probe_req--;
	}

	netif_addr_lock_bh(sdata->dev);
	spin_lock_bh(&local->filter_lock);
	__hw_addr_unsync(&local->mc_list, &sdata->dev->mc,
			 sdata->dev->addr_len);
	spin_unlock_bh(&local->filter_lock);
	netif_addr_unlock_bh(sdata->dev);

	ieee80211_configure_filter(local);

	del_timer_sync(&local->dynamic_ps_timer);
	cancel_work_sync(&local->dynamic_ps_enable_work);

	 
	if (sdata->vif.type == NL80211_IFTYPE_AP) {
		struct ieee80211_sub_if_data *vlan, *tmpsdata;
		struct beacon_data *old_beacon =
			rtnl_dereference(sdata->u.ap.beacon);

		 
		ieee80211_bss_info_change_notify(sdata,
						 BSS_CHANGED_BEACON_ENABLED);

		 
		rcu_assign_pointer(sdata->u.ap.beacon, NULL);
		synchronize_rcu();
		kfree(old_beacon);

		 
		while ((skb = skb_dequeue(&sdata->u.ap.ps_bc_buf))) {
			local->total_ps_buffered--;
			dev_kfree_skb(skb);
		}

		 
		list_for_each_entry_safe(vlan, tmpsdata, &sdata->u.ap.vlans,
					 u.vlan.list)
			dev_close(vlan->dev);
		WARN_ON(!list_empty(&sdata->u.ap.vlans));
	}

	if (going_down)
		local->open_count--;

	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP_VLAN:
		list_del(&sdata->u.vlan.list);
		 
		break;
	case NL80211_IFTYPE_MONITOR:
		if (sdata->u.mntr_flags & MONITOR_FLAG_COOK_FRAMES) {
			local->cooked_mntrs--;
			break;
		}

		local->monitors--;
		if (local->monitors == 0) {
			local->hw.conf.flags &= ~IEEE80211_CONF_MONITOR;
			hw_reconf_flags |= IEEE80211_CONF_CHANGE_MONITOR;
		}

		ieee80211_adjust_monitor_flags(sdata, -1);
		ieee80211_configure_filter(local);
		break;
	default:
		flush_work(&sdata->work);
		 
		synchronize_rcu();
		skb_queue_purge(&sdata->skb_queue);

		 
		if (sdata->vif.type == NL80211_IFTYPE_MESH_POINT)
			ieee80211_bss_info_change_notify(sdata,
				BSS_CHANGED_BEACON_ENABLED);

		 
		ieee80211_free_keys(sdata);

		if (going_down)
			drv_remove_interface(local, &sdata->vif);
	}

	sdata->bss = NULL;

	mutex_lock(&local->mtx);
	hw_reconf_flags |= __ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);

	ieee80211_recalc_ps(local, -1);

	if (local->open_count == 0) {
		if (local->ops->napi_poll)
			napi_disable(&local->napi);
		ieee80211_clear_tx_pending(local);
		ieee80211_stop_device(local);

		 
		hw_reconf_flags = 0;
	}

	 
	orig_ct = local->_oper_channel_type;
	ieee80211_set_channel_type(local, NULL, NL80211_CHAN_NO_HT);

	 
	if (hw_reconf_flags || (orig_ct != local->_oper_channel_type))
		ieee80211_hw_config(local, hw_reconf_flags);

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	for (i = 0; i < IEEE80211_MAX_QUEUES; i++) {
		skb_queue_walk_safe(&local->pending[i], skb, tmp) {
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
			if (info->control.vif == &sdata->vif) {
				__skb_unlink(skb, &local->pending[i]);
				dev_kfree_skb_irq(skb);
			}
		}
	}
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}