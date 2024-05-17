static int nl80211_send_wiphy(struct sk_buff *msg, u32 pid, u32 seq, int flags,
			      struct cfg80211_registered_device *dev)
{
	void *hdr;
	struct nlattr *nl_bands, *nl_band;
	struct nlattr *nl_freqs, *nl_freq;
	struct nlattr *nl_rates, *nl_rate;
	struct nlattr *nl_cmds;
	enum ieee80211_band band;
	struct ieee80211_channel *chan;
	struct ieee80211_rate *rate;
	int i;
	const struct ieee80211_txrx_stypes *mgmt_stypes =
				dev->wiphy.mgmt_stypes;

	hdr = nl80211hdr_put(msg, pid, seq, flags, NL80211_CMD_NEW_WIPHY);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, dev->wiphy_idx);
	NLA_PUT_STRING(msg, NL80211_ATTR_WIPHY_NAME, wiphy_name(&dev->wiphy));

	NLA_PUT_U32(msg, NL80211_ATTR_GENERATION,
		    cfg80211_rdev_list_generation);

	NLA_PUT_U8(msg, NL80211_ATTR_WIPHY_RETRY_SHORT,
		   dev->wiphy.retry_short);
	NLA_PUT_U8(msg, NL80211_ATTR_WIPHY_RETRY_LONG,
		   dev->wiphy.retry_long);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FRAG_THRESHOLD,
		    dev->wiphy.frag_threshold);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_RTS_THRESHOLD,
		    dev->wiphy.rts_threshold);
	NLA_PUT_U8(msg, NL80211_ATTR_WIPHY_COVERAGE_CLASS,
		    dev->wiphy.coverage_class);
	NLA_PUT_U8(msg, NL80211_ATTR_MAX_NUM_SCAN_SSIDS,
		   dev->wiphy.max_scan_ssids);
	NLA_PUT_U16(msg, NL80211_ATTR_MAX_SCAN_IE_LEN,
		    dev->wiphy.max_scan_ie_len);

	if (dev->wiphy.flags & WIPHY_FLAG_IBSS_RSN)
		NLA_PUT_FLAG(msg, NL80211_ATTR_SUPPORT_IBSS_RSN);
	if (dev->wiphy.flags & WIPHY_FLAG_MESH_AUTH)
		NLA_PUT_FLAG(msg, NL80211_ATTR_SUPPORT_MESH_AUTH);

	NLA_PUT(msg, NL80211_ATTR_CIPHER_SUITES,
		sizeof(u32) * dev->wiphy.n_cipher_suites,
		dev->wiphy.cipher_suites);

	NLA_PUT_U8(msg, NL80211_ATTR_MAX_NUM_PMKIDS,
		   dev->wiphy.max_num_pmkids);

	if (dev->wiphy.flags & WIPHY_FLAG_CONTROL_PORT_PROTOCOL)
		NLA_PUT_FLAG(msg, NL80211_ATTR_CONTROL_PORT_ETHERTYPE);

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX,
		    dev->wiphy.available_antennas_tx);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX,
		    dev->wiphy.available_antennas_rx);

	if ((dev->wiphy.available_antennas_tx ||
	     dev->wiphy.available_antennas_rx) && dev->ops->get_antenna) {
		u32 tx_ant = 0, rx_ant = 0;
		int res;
		res = dev->ops->get_antenna(&dev->wiphy, &tx_ant, &rx_ant);
		if (!res) {
			NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_TX, tx_ant);
			NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_RX, rx_ant);
		}
	}

	if (nl80211_put_iftypes(msg, NL80211_ATTR_SUPPORTED_IFTYPES,
				dev->wiphy.interface_modes))
		goto nla_put_failure;

	nl_bands = nla_nest_start(msg, NL80211_ATTR_WIPHY_BANDS);
	if (!nl_bands)
		goto nla_put_failure;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		if (!dev->wiphy.bands[band])
			continue;

		nl_band = nla_nest_start(msg, band);
		if (!nl_band)
			goto nla_put_failure;

		 
		if (dev->wiphy.bands[band]->ht_cap.ht_supported) {
			NLA_PUT(msg, NL80211_BAND_ATTR_HT_MCS_SET,
				sizeof(dev->wiphy.bands[band]->ht_cap.mcs),
				&dev->wiphy.bands[band]->ht_cap.mcs);
			NLA_PUT_U16(msg, NL80211_BAND_ATTR_HT_CAPA,
				dev->wiphy.bands[band]->ht_cap.cap);
			NLA_PUT_U8(msg, NL80211_BAND_ATTR_HT_AMPDU_FACTOR,
				dev->wiphy.bands[band]->ht_cap.ampdu_factor);
			NLA_PUT_U8(msg, NL80211_BAND_ATTR_HT_AMPDU_DENSITY,
				dev->wiphy.bands[band]->ht_cap.ampdu_density);
		}

		 
		nl_freqs = nla_nest_start(msg, NL80211_BAND_ATTR_FREQS);
		if (!nl_freqs)
			goto nla_put_failure;

		for (i = 0; i < dev->wiphy.bands[band]->n_channels; i++) {
			nl_freq = nla_nest_start(msg, i);
			if (!nl_freq)
				goto nla_put_failure;

			chan = &dev->wiphy.bands[band]->channels[i];

			if (nl80211_msg_put_channel(msg, chan))
				goto nla_put_failure;

			nla_nest_end(msg, nl_freq);
		}

		nla_nest_end(msg, nl_freqs);

		 
		nl_rates = nla_nest_start(msg, NL80211_BAND_ATTR_RATES);
		if (!nl_rates)
			goto nla_put_failure;

		for (i = 0; i < dev->wiphy.bands[band]->n_bitrates; i++) {
			nl_rate = nla_nest_start(msg, i);
			if (!nl_rate)
				goto nla_put_failure;

			rate = &dev->wiphy.bands[band]->bitrates[i];
			NLA_PUT_U32(msg, NL80211_BITRATE_ATTR_RATE,
				    rate->bitrate);
			if (rate->flags & IEEE80211_RATE_SHORT_PREAMBLE)
				NLA_PUT_FLAG(msg,
					NL80211_BITRATE_ATTR_2GHZ_SHORTPREAMBLE);

			nla_nest_end(msg, nl_rate);
		}

		nla_nest_end(msg, nl_rates);

		nla_nest_end(msg, nl_band);
	}
	nla_nest_end(msg, nl_bands);

	nl_cmds = nla_nest_start(msg, NL80211_ATTR_SUPPORTED_COMMANDS);
	if (!nl_cmds)
		goto nla_put_failure;

	i = 0;
#define CMD(op, n)						\
	 do {							\
		if (dev->ops->op) {				\
			i++;					\
			NLA_PUT_U32(msg, i, NL80211_CMD_ ## n);	\
		}						\
	} while (0)

	CMD(add_virtual_intf, NEW_INTERFACE);
	CMD(change_virtual_intf, SET_INTERFACE);
	CMD(add_key, NEW_KEY);
	CMD(add_beacon, NEW_BEACON);
	CMD(add_station, NEW_STATION);
	CMD(add_mpath, NEW_MPATH);
	CMD(update_mesh_config, SET_MESH_CONFIG);
	CMD(change_bss, SET_BSS);
	CMD(auth, AUTHENTICATE);
	CMD(assoc, ASSOCIATE);
	CMD(deauth, DEAUTHENTICATE);
	CMD(disassoc, DISASSOCIATE);
	CMD(join_ibss, JOIN_IBSS);
	CMD(join_mesh, JOIN_MESH);
	CMD(set_pmksa, SET_PMKSA);
	CMD(del_pmksa, DEL_PMKSA);
	CMD(flush_pmksa, FLUSH_PMKSA);
	CMD(remain_on_channel, REMAIN_ON_CHANNEL);
	CMD(set_bitrate_mask, SET_TX_BITRATE_MASK);
	CMD(mgmt_tx, FRAME);
	CMD(mgmt_tx_cancel_wait, FRAME_WAIT_CANCEL);
	if (dev->wiphy.flags & WIPHY_FLAG_NETNS_OK) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_SET_WIPHY_NETNS);
	}
	CMD(set_channel, SET_CHANNEL);
	CMD(set_wds_peer, SET_WDS_PEER);
	if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_SCHED_SCAN)
		CMD(sched_scan_start, START_SCHED_SCAN);

#undef CMD

	if (dev->ops->connect || dev->ops->auth) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_CONNECT);
	}

	if (dev->ops->disconnect || dev->ops->deauth) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_DISCONNECT);
	}

	nla_nest_end(msg, nl_cmds);

	if (dev->ops->remain_on_channel)
		NLA_PUT_U32(msg, NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION,
			    dev->wiphy.max_remain_on_channel_duration);

	 
	if (dev->ops->mgmt_tx)
		NLA_PUT_FLAG(msg, NL80211_ATTR_OFFCHANNEL_TX_OK);

	if (mgmt_stypes) {
		u16 stypes;
		struct nlattr *nl_ftypes, *nl_ifs;
		enum nl80211_iftype ift;

		nl_ifs = nla_nest_start(msg, NL80211_ATTR_TX_FRAME_TYPES);
		if (!nl_ifs)
			goto nla_put_failure;

		for (ift = 0; ift < NUM_NL80211_IFTYPES; ift++) {
			nl_ftypes = nla_nest_start(msg, ift);
			if (!nl_ftypes)
				goto nla_put_failure;
			i = 0;
			stypes = mgmt_stypes[ift].tx;
			while (stypes) {
				if (stypes & 1)
					NLA_PUT_U16(msg, NL80211_ATTR_FRAME_TYPE,
						    (i << 4) | IEEE80211_FTYPE_MGMT);
				stypes >>= 1;
				i++;
			}
			nla_nest_end(msg, nl_ftypes);
		}

		nla_nest_end(msg, nl_ifs);

		nl_ifs = nla_nest_start(msg, NL80211_ATTR_RX_FRAME_TYPES);
		if (!nl_ifs)
			goto nla_put_failure;

		for (ift = 0; ift < NUM_NL80211_IFTYPES; ift++) {
			nl_ftypes = nla_nest_start(msg, ift);
			if (!nl_ftypes)
				goto nla_put_failure;
			i = 0;
			stypes = mgmt_stypes[ift].rx;
			while (stypes) {
				if (stypes & 1)
					NLA_PUT_U16(msg, NL80211_ATTR_FRAME_TYPE,
						    (i << 4) | IEEE80211_FTYPE_MGMT);
				stypes >>= 1;
				i++;
			}
			nla_nest_end(msg, nl_ftypes);
		}
		nla_nest_end(msg, nl_ifs);
	}

	if (dev->wiphy.wowlan.flags || dev->wiphy.wowlan.n_patterns) {
		struct nlattr *nl_wowlan;

		nl_wowlan = nla_nest_start(msg,
				NL80211_ATTR_WOWLAN_TRIGGERS_SUPPORTED);
		if (!nl_wowlan)
			goto nla_put_failure;

		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_ANY)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_ANY);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_DISCONNECT)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_DISCONNECT);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_MAGIC_PKT)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_MAGIC_PKT);
		if (dev->wiphy.wowlan.n_patterns) {
			struct nl80211_wowlan_pattern_support pat = {
				.max_patterns = dev->wiphy.wowlan.n_patterns,
				.min_pattern_len =
					dev->wiphy.wowlan.pattern_min_len,
				.max_pattern_len =
					dev->wiphy.wowlan.pattern_max_len,
			};
			NLA_PUT(msg, NL80211_WOWLAN_TRIG_PKT_PATTERN,
				sizeof(pat), &pat);
		}

		nla_nest_end(msg, nl_wowlan);
	}

	if (nl80211_put_iftypes(msg, NL80211_ATTR_SOFTWARE_IFTYPES,
				dev->wiphy.software_iftypes))
		goto nla_put_failure;

	if (nl80211_put_iface_combinations(&dev->wiphy, msg))
		goto nla_put_failure;

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}