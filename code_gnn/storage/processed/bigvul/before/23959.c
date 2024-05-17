static int airo_get_range(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *dwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;
	struct iw_range *range = (struct iw_range *) extra;
	CapabilityRid cap_rid;		 
	int		i;
	int		k;

	readCapabilityRid(local, &cap_rid, 1);

	dwrq->length = sizeof(struct iw_range);
	memset(range, 0, sizeof(*range));
	range->min_nwid = 0x0000;
	range->max_nwid = 0x0000;
	range->num_channels = 14;
	 
	k = 0;
	for(i = 0; i < 14; i++) {
		range->freq[k].i = i + 1;  
		range->freq[k].m = ieee80211_dsss_chan_to_freq(i + 1) * 100000;
		range->freq[k++].e = 1;	 
	}
	range->num_frequency = k;

	range->sensitivity = 65535;

	 
	if (local->rssi)
		range->max_qual.qual = 100;	 
	else
		range->max_qual.qual = airo_get_max_quality(&cap_rid);
	range->max_qual.level = 0x100 - 120;	 
	range->max_qual.noise = 0x100 - 120;	 

	 
	 
	if (local->rssi) {
		range->avg_qual.qual = 50;		 
		range->avg_qual.level = 0x100 - 70;	 
	} else {
		range->avg_qual.qual = airo_get_avg_quality(&cap_rid);
		range->avg_qual.level = 0x100 - 80;	 
	}
	range->avg_qual.noise = 0x100 - 85;		 

	for(i = 0 ; i < 8 ; i++) {
		range->bitrate[i] = cap_rid.supportedRates[i] * 500000;
		if(range->bitrate[i] == 0)
			break;
	}
	range->num_bitrates = i;

	 
	if(i > 2)
		range->throughput = 5000 * 1000;
	else
		range->throughput = 1500 * 1000;

	range->min_rts = 0;
	range->max_rts = AIRO_DEF_MTU;
	range->min_frag = 256;
	range->max_frag = AIRO_DEF_MTU;

	if(cap_rid.softCap & cpu_to_le16(2)) {
		range->encoding_size[0] = 5;
		if (cap_rid.softCap & cpu_to_le16(0x100)) {
			range->encoding_size[1] = 13;
			range->num_encoding_sizes = 2;
		} else
			range->num_encoding_sizes = 1;
		range->max_encoding_tokens =
			cap_rid.softCap & cpu_to_le16(0x80) ? 4 : 1;
	} else {
		range->num_encoding_sizes = 0;
		range->max_encoding_tokens = 0;
	}
	range->min_pmp = 0;
	range->max_pmp = 5000000;	 
	range->min_pmt = 0;
	range->max_pmt = 65535 * 1024;	 
	range->pmp_flags = IW_POWER_PERIOD;
	range->pmt_flags = IW_POWER_TIMEOUT;
	range->pm_capa = IW_POWER_PERIOD | IW_POWER_TIMEOUT | IW_POWER_ALL_R;

	 
	for(i = 0 ; i < 8 ; i++) {
		range->txpower[i] = le16_to_cpu(cap_rid.txPowerLevels[i]);
		if(range->txpower[i] == 0)
			break;
	}
	range->num_txpower = i;
	range->txpower_capa = IW_TXPOW_MWATT;
	range->we_version_source = 19;
	range->we_version_compiled = WIRELESS_EXT;
	range->retry_capa = IW_RETRY_LIMIT | IW_RETRY_LIFETIME;
	range->retry_flags = IW_RETRY_LIMIT;
	range->r_time_flags = IW_RETRY_LIFETIME;
	range->min_retry = 1;
	range->max_retry = 65535;
	range->min_r_time = 1024;
	range->max_r_time = 65535 * 1024;

	 
	range->event_capa[0] = (IW_EVENT_CAPA_K_0 |
				IW_EVENT_CAPA_MASK(SIOCGIWTHRSPY) |
				IW_EVENT_CAPA_MASK(SIOCGIWAP) |
				IW_EVENT_CAPA_MASK(SIOCGIWSCAN));
	range->event_capa[1] = IW_EVENT_CAPA_K_1;
	range->event_capa[4] = IW_EVENT_CAPA_MASK(IWEVTXDROP);
	return 0;
}