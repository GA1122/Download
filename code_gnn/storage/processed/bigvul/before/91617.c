struct attr *bgp_attr_aggregate_intern(struct bgp *bgp, uint8_t origin,
				       struct aspath *aspath,
				       struct community *community,
				       struct ecommunity *ecommunity,
				       struct lcommunity *lcommunity,
				       int as_set, uint8_t atomic_aggregate)
{
	struct attr attr;
	struct attr *new;

	memset(&attr, 0, sizeof(struct attr));

	 
	attr.origin = origin;
	attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_ORIGIN);

	 
	if (aspath)
		attr.aspath = aspath_intern(aspath);
	else
		attr.aspath = aspath_empty();
	attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_AS_PATH);

	 
	attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_NEXT_HOP);

	if (community) {
		uint32_t gshut = COMMUNITY_GSHUT;

		 
		if (!bgp_flag_check(bgp, BGP_FLAG_GRACEFUL_SHUTDOWN)
		    && community_include(community, gshut)) {
			community_del_val(community, &gshut);
		}

		attr.community = community;
		attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_COMMUNITIES);
	}

	if (ecommunity) {
		attr.ecommunity = ecommunity;
		attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_EXT_COMMUNITIES);
	}

	if (lcommunity) {
		attr.lcommunity = lcommunity;
		attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_LARGE_COMMUNITIES);
	}

	if (bgp_flag_check(bgp, BGP_FLAG_GRACEFUL_SHUTDOWN)) {
		bgp_attr_add_gshut_community(&attr);
	}

	attr.label_index = BGP_INVALID_LABEL_INDEX;
	attr.label = MPLS_INVALID_LABEL;
	attr.weight = BGP_ATTR_DEFAULT_WEIGHT;
	attr.mp_nexthop_len = IPV6_MAX_BYTELEN;
	if (!as_set || atomic_aggregate)
		attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_ATOMIC_AGGREGATE);
	attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_AGGREGATOR);
	if (CHECK_FLAG(bgp->config, BGP_CONFIG_CONFEDERATION))
		attr.aggregator_as = bgp->confed_id;
	else
		attr.aggregator_as = bgp->as;
	attr.aggregator_addr = bgp->router_id;
	attr.label_index = BGP_INVALID_LABEL_INDEX;
	attr.label = MPLS_INVALID_LABEL;

	new = bgp_attr_intern(&attr);

	aspath_unintern(&new->aspath);
	return new;
}