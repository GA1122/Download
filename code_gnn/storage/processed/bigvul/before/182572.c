  int hns_rcb_get_ring_sset_count(int stringset)
  {
	if (stringset == ETH_SS_STATS)
// 	if (stringset == ETH_SS_STATS || stringset == ETH_SS_PRIV_FLAGS)
  		return HNS_RING_STATIC_REG_NUM;
  
  	return 0;
 }