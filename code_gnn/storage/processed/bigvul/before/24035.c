static void get_tx_error(struct airo_info *ai, s32 fid)
{
	__le16 status;

	if (fid < 0)
		status = ((WifiCtlHdr *)ai->txfids[0].virtual_host_addr)->ctlhdr.status;
	else {
		if (bap_setup(ai, ai->fids[fid] & 0xffff, 4, BAP0) != SUCCESS)
			return;
		bap_read(ai, &status, 2, BAP0);
	}
	if (le16_to_cpu(status) & 2)  
		ai->dev->stats.tx_aborted_errors++;
	if (le16_to_cpu(status) & 4)  
		ai->dev->stats.tx_heartbeat_errors++;
	if (le16_to_cpu(status) & 8)  
		{ }
	if (le16_to_cpu(status) & 0x10)  
		ai->dev->stats.tx_carrier_errors++;
	if (le16_to_cpu(status) & 0x20)  
		{ }
	 
	if ((le16_to_cpu(status) & 2) ||
	     (le16_to_cpu(status) & 4)) {
		union iwreq_data	wrqu;
		char junk[0x18];

		 
		bap_read(ai, (__le16 *) junk, 0x18, BAP0);

		 
		memcpy(wrqu.addr.sa_data, junk + 0x12, ETH_ALEN);
		wrqu.addr.sa_family = ARPHRD_ETHER;

		 
		wireless_send_event(ai->dev, IWEVTXDROP, &wrqu, NULL);
	}
}
