ieee80211_sta_ps_deliver_response(struct sta_info *sta,
				  int n_frames, u8 ignored_acs,
				  enum ieee80211_frame_release_type reason)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ieee80211_local *local = sdata->local;
	bool more_data = false;
	int ac;
	unsigned long driver_release_tids = 0;
	struct sk_buff_head frames;

	 
	set_sta_flag(sta, WLAN_STA_SP);

	__skb_queue_head_init(&frames);

	 
	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		unsigned long tids;

		if (ignored_acs & BIT(ac))
			continue;

		tids = ieee80211_tids_for_ac(ac);

		 
		if (skb_queue_empty(&frames))
			driver_release_tids |= sta->driver_buffered_tids & tids;

		if (driver_release_tids) {
			 
			if (reason == IEEE80211_FRAME_RELEASE_PSPOLL &&
			    hweight16(driver_release_tids) > 1) {
				more_data = true;
				driver_release_tids =
					BIT(find_highest_prio_tid(
						driver_release_tids));
				break;
			}
		} else {
			struct sk_buff *skb;

			while (n_frames > 0) {
				skb = skb_dequeue(&sta->tx_filtered[ac]);
				if (!skb) {
					skb = skb_dequeue(
						&sta->ps_tx_buf[ac]);
					if (skb)
						local->total_ps_buffered--;
				}
				if (!skb)
					break;
				n_frames--;
				__skb_queue_tail(&frames, skb);
			}
		}

		 
		if (!skb_queue_empty(&sta->tx_filtered[ac]) ||
		    !skb_queue_empty(&sta->ps_tx_buf[ac])) {
			more_data = true;
			break;
		}
	}

	if (skb_queue_empty(&frames) && !driver_release_tids) {
		int tid;

		 

		 
		tid = 7 - ((ffs(~ignored_acs) - 1) << 1);

		ieee80211_send_null_response(sdata, sta, tid, reason, true);
	} else if (!driver_release_tids) {
		struct sk_buff_head pending;
		struct sk_buff *skb;
		int num = 0;
		u16 tids = 0;
		bool need_null = false;

		skb_queue_head_init(&pending);

		while ((skb = __skb_dequeue(&frames))) {
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
			struct ieee80211_hdr *hdr = (void *) skb->data;
			u8 *qoshdr = NULL;

			num++;

			 
			info->flags |= IEEE80211_TX_CTL_NO_PS_BUFFER |
				       IEEE80211_TX_CTL_PS_RESPONSE;

			 
			if (more_data || !skb_queue_empty(&frames))
				hdr->frame_control |=
					cpu_to_le16(IEEE80211_FCTL_MOREDATA);
			else
				hdr->frame_control &=
					cpu_to_le16(~IEEE80211_FCTL_MOREDATA);

			if (ieee80211_is_data_qos(hdr->frame_control) ||
			    ieee80211_is_qos_nullfunc(hdr->frame_control))
				qoshdr = ieee80211_get_qos_ctl(hdr);

			tids |= BIT(skb->priority);

			__skb_queue_tail(&pending, skb);

			 
			if (!skb_queue_empty(&frames))
				continue;

			if (reason != IEEE80211_FRAME_RELEASE_UAPSD) {
				 
				info->flags |= IEEE80211_TX_STATUS_EOSP |
					       IEEE80211_TX_CTL_REQ_TX_STATUS;
				break;
			}

			 
			if (qoshdr) {
				*qoshdr |= IEEE80211_QOS_CTL_EOSP;

				info->flags |= IEEE80211_TX_STATUS_EOSP |
					       IEEE80211_TX_CTL_REQ_TX_STATUS;
			} else {
				 
				hdr->frame_control |=
					cpu_to_le16(IEEE80211_FCTL_MOREDATA);
				need_null = true;
				num++;
			}
			break;
		}

		drv_allow_buffered_frames(local, sta, tids, num,
					  reason, more_data);

		ieee80211_add_pending_skbs(local, &pending);

		if (need_null)
			ieee80211_send_null_response(
				sdata, sta, find_highest_prio_tid(tids),
				reason, false);

		sta_info_recalc_tim(sta);
	} else {
		 
		drv_release_buffered_frames(local, sta, driver_release_tids,
					    n_frames, reason, more_data);

		 
	}
}
