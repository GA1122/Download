static __le16 ieee80211_duration(struct ieee80211_tx_data *tx,
				 struct sk_buff *skb, int group_addr,
				 int next_frag_len)
{
	int rate, mrate, erp, dur, i, shift = 0;
	struct ieee80211_rate *txrate;
	struct ieee80211_local *local = tx->local;
	struct ieee80211_supported_band *sband;
	struct ieee80211_hdr *hdr;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	struct ieee80211_chanctx_conf *chanctx_conf;
	u32 rate_flags = 0;

	rcu_read_lock();
	chanctx_conf = rcu_dereference(tx->sdata->vif.chanctx_conf);
	if (chanctx_conf) {
		shift = ieee80211_chandef_get_shift(&chanctx_conf->def);
		rate_flags = ieee80211_chandef_rate_flags(&chanctx_conf->def);
	}
	rcu_read_unlock();

	 
	if (tx->rate.flags & IEEE80211_TX_RC_MCS)
		return 0;

	 
	if (WARN_ON_ONCE(tx->rate.idx < 0))
		return 0;

	sband = local->hw.wiphy->bands[info->band];
	txrate = &sband->bitrates[tx->rate.idx];

	erp = txrate->flags & IEEE80211_RATE_ERP_G;

	 
	hdr = (struct ieee80211_hdr *)skb->data;
	if (ieee80211_is_ctl(hdr->frame_control)) {
		 
		return 0;
	}

	 
	if (0  )
		return cpu_to_le16(32768);

	if (group_addr)  
		return 0;

	 
	rate = -1;
	 
	mrate = sband->bitrates[0].bitrate;
	for (i = 0; i < sband->n_bitrates; i++) {
		struct ieee80211_rate *r = &sband->bitrates[i];

		if (r->bitrate > txrate->bitrate)
			break;

		if ((rate_flags & r->flags) != rate_flags)
			continue;

		if (tx->sdata->vif.bss_conf.basic_rates & BIT(i))
			rate = DIV_ROUND_UP(r->bitrate, 1 << shift);

		switch (sband->band) {
		case IEEE80211_BAND_2GHZ: {
			u32 flag;
			if (tx->sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
				flag = IEEE80211_RATE_MANDATORY_G;
			else
				flag = IEEE80211_RATE_MANDATORY_B;
			if (r->flags & flag)
				mrate = r->bitrate;
			break;
		}
		case IEEE80211_BAND_5GHZ:
			if (r->flags & IEEE80211_RATE_MANDATORY_A)
				mrate = r->bitrate;
			break;
		case IEEE80211_BAND_60GHZ:
			 
		case IEEE80211_NUM_BANDS:
			WARN_ON(1);
			break;
		}
	}
	if (rate == -1) {
		 
		rate = DIV_ROUND_UP(mrate, 1 << shift);
	}

	 
	if (ieee80211_is_data_qos(hdr->frame_control) &&
	    *(ieee80211_get_qos_ctl(hdr)) & IEEE80211_QOS_CTL_ACK_POLICY_NOACK)
		dur = 0;
	else
		 
		dur = ieee80211_frame_duration(sband->band, 10, rate, erp,
				tx->sdata->vif.bss_conf.use_short_preamble,
				shift);

	if (next_frag_len) {
		 
		dur *= 2;  
		 
		dur += ieee80211_frame_duration(sband->band, next_frag_len,
				txrate->bitrate, erp,
				tx->sdata->vif.bss_conf.use_short_preamble,
				shift);
	}

	return cpu_to_le16(dur);
}
