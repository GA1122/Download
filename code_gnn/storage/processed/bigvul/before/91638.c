struct attr *bgp_attr_intern(struct attr *attr)
{
	struct attr *find;

	 
	if (attr->aspath) {
		if (!attr->aspath->refcnt)
			attr->aspath = aspath_intern(attr->aspath);
		else
			attr->aspath->refcnt++;
	}
	if (attr->community) {
		if (!attr->community->refcnt)
			attr->community = community_intern(attr->community);
		else
			attr->community->refcnt++;
	}

	if (attr->ecommunity) {
		if (!attr->ecommunity->refcnt)
			attr->ecommunity = ecommunity_intern(attr->ecommunity);
		else
			attr->ecommunity->refcnt++;
	}
	if (attr->lcommunity) {
		if (!attr->lcommunity->refcnt)
			attr->lcommunity = lcommunity_intern(attr->lcommunity);
		else
			attr->lcommunity->refcnt++;
	}
	if (attr->cluster) {
		if (!attr->cluster->refcnt)
			attr->cluster = cluster_intern(attr->cluster);
		else
			attr->cluster->refcnt++;
	}
	if (attr->transit) {
		if (!attr->transit->refcnt)
			attr->transit = transit_intern(attr->transit);
		else
			attr->transit->refcnt++;
	}
	if (attr->encap_subtlvs) {
		if (!attr->encap_subtlvs->refcnt)
			attr->encap_subtlvs = encap_intern(attr->encap_subtlvs,
							   ENCAP_SUBTLV_TYPE);
		else
			attr->encap_subtlvs->refcnt++;
	}
#if ENABLE_BGP_VNC
	if (attr->vnc_subtlvs) {
		if (!attr->vnc_subtlvs->refcnt)
			attr->vnc_subtlvs = encap_intern(attr->vnc_subtlvs,
							 VNC_SUBTLV_TYPE);
		else
			attr->vnc_subtlvs->refcnt++;
	}
#endif

	 
	find = (struct attr *)hash_get(attrhash, attr, bgp_attr_hash_alloc);
	find->refcnt++;

	return find;
}
