static void ieee80211_iface_work(struct work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data, work);
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct sta_info *sta;
	struct ieee80211_ra_tid *ra_tid;

	if (!ieee80211_sdata_running(sdata))
		return;

	if (local->scanning)
		return;

	 
	if (WARN(local->suspended,
		 "interface work scheduled while going to suspend\n"))
		return;

	 
	while ((skb = skb_dequeue(&sdata->skb_queue))) {
		struct ieee80211_mgmt *mgmt = (void *)skb->data;

		if (skb->pkt_type == IEEE80211_SDATA_QUEUE_AGG_START) {
			ra_tid = (void *)&skb->cb;
			ieee80211_start_tx_ba_cb(&sdata->vif, ra_tid->ra,
						 ra_tid->tid);
		} else if (skb->pkt_type == IEEE80211_SDATA_QUEUE_AGG_STOP) {
			ra_tid = (void *)&skb->cb;
			ieee80211_stop_tx_ba_cb(&sdata->vif, ra_tid->ra,
						ra_tid->tid);
		} else if (ieee80211_is_action(mgmt->frame_control) &&
			   mgmt->u.action.category == WLAN_CATEGORY_BACK) {
			int len = skb->len;

			mutex_lock(&local->sta_mtx);
			sta = sta_info_get_bss(sdata, mgmt->sa);
			if (sta) {
				switch (mgmt->u.action.u.addba_req.action_code) {
				case WLAN_ACTION_ADDBA_REQ:
					ieee80211_process_addba_request(
							local, sta, mgmt, len);
					break;
				case WLAN_ACTION_ADDBA_RESP:
					ieee80211_process_addba_resp(local, sta,
								     mgmt, len);
					break;
				case WLAN_ACTION_DELBA:
					ieee80211_process_delba(sdata, sta,
								mgmt, len);
					break;
				default:
					WARN_ON(1);
					break;
				}
			}
			mutex_unlock(&local->sta_mtx);
		} else if (ieee80211_is_data_qos(mgmt->frame_control)) {
			struct ieee80211_hdr *hdr = (void *)mgmt;
			 
			WARN_ON(hdr->frame_control &
					cpu_to_le16(IEEE80211_STYPE_NULLFUNC));
			WARN_ON(!(hdr->seq_ctrl &
					cpu_to_le16(IEEE80211_SCTL_FRAG)));
			 
			mutex_lock(&local->sta_mtx);
			sta = sta_info_get_bss(sdata, mgmt->sa);
			if (sta) {
				u16 tid = *ieee80211_get_qos_ctl(hdr) &
						IEEE80211_QOS_CTL_TID_MASK;

				__ieee80211_stop_rx_ba_session(
					sta, tid, WLAN_BACK_RECIPIENT,
					WLAN_REASON_QSTA_REQUIRE_SETUP,
					true);
			}
			mutex_unlock(&local->sta_mtx);
		} else switch (sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			ieee80211_sta_rx_queued_mgmt(sdata, skb);
			break;
		case NL80211_IFTYPE_ADHOC:
			ieee80211_ibss_rx_queued_mgmt(sdata, skb);
			break;
		case NL80211_IFTYPE_MESH_POINT:
			if (!ieee80211_vif_is_mesh(&sdata->vif))
				break;
			ieee80211_mesh_rx_queued_mgmt(sdata, skb);
			break;
		default:
			WARN(1, "frame for unexpected interface type");
			break;
		}

		kfree_skb(skb);
	}

	 
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
		ieee80211_sta_work(sdata);
		break;
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_work(sdata);
		break;
	case NL80211_IFTYPE_MESH_POINT:
		if (!ieee80211_vif_is_mesh(&sdata->vif))
			break;
		ieee80211_mesh_work(sdata);
		break;
	default:
		break;
	}
}