static void airo_read_wireless_stats(struct airo_info *local)
{
	StatusRid status_rid;
	StatsRid stats_rid;
	CapabilityRid cap_rid;
	__le32 *vals = stats_rid.vals;

	 
	clear_bit(JOB_WSTATS, &local->jobs);
	if (local->power.event) {
		up(&local->sem);
		return;
	}
	readCapabilityRid(local, &cap_rid, 0);
	readStatusRid(local, &status_rid, 0);
	readStatsRid(local, &stats_rid, RID_STATS, 0);
	up(&local->sem);

	 
	local->wstats.status = le16_to_cpu(status_rid.mode);

	 
	if (local->rssi) {
		local->wstats.qual.level =
			airo_rssi_to_dbm(local->rssi,
					 le16_to_cpu(status_rid.sigQuality));
		 
		local->wstats.qual.qual =
			le16_to_cpu(status_rid.normalizedSignalStrength);
	} else {
		local->wstats.qual.level =
			(le16_to_cpu(status_rid.normalizedSignalStrength) + 321) / 2;
		local->wstats.qual.qual = airo_get_quality(&status_rid, &cap_rid);
	}
	if (le16_to_cpu(status_rid.len) >= 124) {
		local->wstats.qual.noise = 0x100 - status_rid.noisedBm;
		local->wstats.qual.updated = IW_QUAL_ALL_UPDATED | IW_QUAL_DBM;
	} else {
		local->wstats.qual.noise = 0;
		local->wstats.qual.updated = IW_QUAL_QUAL_UPDATED | IW_QUAL_LEVEL_UPDATED | IW_QUAL_NOISE_INVALID | IW_QUAL_DBM;
	}

	 
	local->wstats.discard.nwid = le32_to_cpu(vals[56]) +
				     le32_to_cpu(vals[57]) +
				     le32_to_cpu(vals[58]);  
	local->wstats.discard.code = le32_to_cpu(vals[6]); 
	local->wstats.discard.fragment = le32_to_cpu(vals[30]);
	local->wstats.discard.retries = le32_to_cpu(vals[10]);
	local->wstats.discard.misc = le32_to_cpu(vals[1]) +
				     le32_to_cpu(vals[32]);
	local->wstats.miss.beacon = le32_to_cpu(vals[34]);
}