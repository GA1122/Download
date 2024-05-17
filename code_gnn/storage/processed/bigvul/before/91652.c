void bgp_attr_unintern_sub(struct attr *attr)
{
	 
	if (attr->aspath)
		aspath_unintern(&attr->aspath);
	UNSET_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_AS_PATH));

	if (attr->community)
		community_unintern(&attr->community);
	UNSET_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_COMMUNITIES));

	if (attr->ecommunity)
		ecommunity_unintern(&attr->ecommunity);
	UNSET_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_EXT_COMMUNITIES));

	if (attr->lcommunity)
		lcommunity_unintern(&attr->lcommunity);
	UNSET_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_LARGE_COMMUNITIES));

	if (attr->cluster)
		cluster_unintern(attr->cluster);
	UNSET_FLAG(attr->flag, ATTR_FLAG_BIT(BGP_ATTR_CLUSTER_LIST));

	if (attr->transit)
		transit_unintern(attr->transit);

	if (attr->encap_subtlvs)
		encap_unintern(&attr->encap_subtlvs, ENCAP_SUBTLV_TYPE);

#if ENABLE_BGP_VNC
	if (attr->vnc_subtlvs)
		encap_unintern(&attr->vnc_subtlvs, VNC_SUBTLV_TYPE);
#endif
}