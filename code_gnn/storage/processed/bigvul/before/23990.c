static int airo_set_essid(struct net_device *dev,
			  struct iw_request_info *info,
			  struct iw_point *dwrq,
			  char *extra)
{
	struct airo_info *local = dev->ml_priv;
	SsidRid SSID_rid;		 

	 
	readSsidRid(local, &SSID_rid);

	 
	if (dwrq->flags == 0) {
		 
		memset(&SSID_rid, 0, sizeof(SSID_rid));
	} else {
		unsigned index = (dwrq->flags & IW_ENCODE_INDEX) - 1;

		 
		if (dwrq->length > IW_ESSID_MAX_SIZE)
			return -E2BIG ;

		 
		if (index >= ARRAY_SIZE(SSID_rid.ssids))
			return -EINVAL;

		 
		memset(SSID_rid.ssids[index].ssid, 0,
		       sizeof(SSID_rid.ssids[index].ssid));
		memcpy(SSID_rid.ssids[index].ssid, extra, dwrq->length);
		SSID_rid.ssids[index].len = cpu_to_le16(dwrq->length);
	}
	SSID_rid.len = cpu_to_le16(sizeof(SSID_rid));
	 
	disable_MAC(local, 1);
	writeSsidRid(local, &SSID_rid, 1);
	enable_MAC(local, 1);

	return 0;
}
