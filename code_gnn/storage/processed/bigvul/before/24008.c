static inline char *airo_translate_scan(struct net_device *dev,
					struct iw_request_info *info,
					char *current_ev,
					char *end_buf,
					BSSListRid *bss)
{
	struct airo_info *ai = dev->ml_priv;
	struct iw_event		iwe;		 
	__le16			capabilities;
	char *			current_val;	 
	int			i;
	char *		buf;
	u16 dBm;

	 
	iwe.cmd = SIOCGIWAP;
	iwe.u.ap_addr.sa_family = ARPHRD_ETHER;
	memcpy(iwe.u.ap_addr.sa_data, bss->bssid, ETH_ALEN);
	current_ev = iwe_stream_add_event(info, current_ev, end_buf,
					  &iwe, IW_EV_ADDR_LEN);

	 

	 
	iwe.u.data.length = bss->ssidLen;
	if(iwe.u.data.length > 32)
		iwe.u.data.length = 32;
	iwe.cmd = SIOCGIWESSID;
	iwe.u.data.flags = 1;
	current_ev = iwe_stream_add_point(info, current_ev, end_buf,
					  &iwe, bss->ssid);

	 
	iwe.cmd = SIOCGIWMODE;
	capabilities = bss->cap;
	if(capabilities & (CAP_ESS | CAP_IBSS)) {
		if(capabilities & CAP_ESS)
			iwe.u.mode = IW_MODE_MASTER;
		else
			iwe.u.mode = IW_MODE_ADHOC;
		current_ev = iwe_stream_add_event(info, current_ev, end_buf,
						  &iwe, IW_EV_UINT_LEN);
	}

	 
	iwe.cmd = SIOCGIWFREQ;
	iwe.u.freq.m = le16_to_cpu(bss->dsChannel);
	iwe.u.freq.m = ieee80211_dsss_chan_to_freq(iwe.u.freq.m) * 100000;
	iwe.u.freq.e = 1;
	current_ev = iwe_stream_add_event(info, current_ev, end_buf,
					  &iwe, IW_EV_FREQ_LEN);

	dBm = le16_to_cpu(bss->dBm);

	 
	iwe.cmd = IWEVQUAL;
	if (ai->rssi) {
		iwe.u.qual.level = 0x100 - dBm;
		iwe.u.qual.qual = airo_dbm_to_pct(ai->rssi, dBm);
		iwe.u.qual.updated = IW_QUAL_QUAL_UPDATED
				| IW_QUAL_LEVEL_UPDATED
				| IW_QUAL_DBM;
	} else {
		iwe.u.qual.level = (dBm + 321) / 2;
		iwe.u.qual.qual = 0;
		iwe.u.qual.updated = IW_QUAL_QUAL_INVALID
				| IW_QUAL_LEVEL_UPDATED
				| IW_QUAL_DBM;
	}
	iwe.u.qual.noise = ai->wstats.qual.noise;
	current_ev = iwe_stream_add_event(info, current_ev, end_buf,
					  &iwe, IW_EV_QUAL_LEN);

	 
	iwe.cmd = SIOCGIWENCODE;
	if(capabilities & CAP_PRIVACY)
		iwe.u.data.flags = IW_ENCODE_ENABLED | IW_ENCODE_NOKEY;
	else
		iwe.u.data.flags = IW_ENCODE_DISABLED;
	iwe.u.data.length = 0;
	current_ev = iwe_stream_add_point(info, current_ev, end_buf,
					  &iwe, bss->ssid);

	 
	current_val = current_ev + iwe_stream_lcp_len(info);

	iwe.cmd = SIOCGIWRATE;
	 
	iwe.u.bitrate.fixed = iwe.u.bitrate.disabled = 0;
	 
	for(i = 0 ; i < 8 ; i++) {
		 
		if(bss->rates[i] == 0)
			break;
		 
		iwe.u.bitrate.value = ((bss->rates[i] & 0x7f) * 500000);
		 
		current_val = iwe_stream_add_value(info, current_ev,
						   current_val, end_buf,
						   &iwe, IW_EV_PARAM_LEN);
	}
	 
	if ((current_val - current_ev) > iwe_stream_lcp_len(info))
		current_ev = current_val;

	 
	buf = kmalloc(30, GFP_KERNEL);
	if (buf) {
		iwe.cmd = IWEVCUSTOM;
		sprintf(buf, "bcn_int=%d", bss->beaconInterval);
		iwe.u.data.length = strlen(buf);
		current_ev = iwe_stream_add_point(info, current_ev, end_buf,
						  &iwe, buf);
		kfree(buf);
	}

	 
	if (test_bit(FLAG_WPA_CAPABLE, &ai->flags)) {
		unsigned int num_null_ies = 0;
		u16 length = sizeof (bss->extra.iep);
		u8 *ie = (void *)&bss->extra.iep;

		while ((length >= 2) && (num_null_ies < 2)) {
			if (2 + ie[1] > length) {
				 
				break;
			}

			switch (ie[0]) {
			case WLAN_EID_SSID:
				 
				if (!ie[1])
					num_null_ies++;
				break;

			case WLAN_EID_GENERIC:
				if (ie[1] >= 4 &&
				    ie[2] == 0x00 &&
				    ie[3] == 0x50 &&
				    ie[4] == 0xf2 &&
				    ie[5] == 0x01) {
					iwe.cmd = IWEVGENIE;
					 
					iwe.u.data.length = min(ie[1] + 2,
								64);
					current_ev = iwe_stream_add_point(
							info, current_ev,
							end_buf, &iwe, ie);
				}
				break;

			case WLAN_EID_RSN:
				iwe.cmd = IWEVGENIE;
				 
				iwe.u.data.length = min(ie[1] + 2, 64);
				current_ev = iwe_stream_add_point(
					info, current_ev, end_buf,
					&iwe, ie);
				break;

			default:
				break;
			}

			length -= 2 + ie[1];
			ie += 2 + ie[1];
		}
	}
	return current_ev;
}
