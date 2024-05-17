static int airo_get_wap(struct net_device *dev,
			struct iw_request_info *info,
			struct sockaddr *awrq,
			char *extra)
{
	struct airo_info *local = dev->ml_priv;
	StatusRid status_rid;		 

	readStatusRid(local, &status_rid, 1);

	 
	memcpy(awrq->sa_data, status_rid.bssid[0], ETH_ALEN);
	awrq->sa_family = ARPHRD_ETHER;

	return 0;
}
