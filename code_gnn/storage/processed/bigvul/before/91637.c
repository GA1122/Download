void bgp_attr_init(void)
{
	aspath_init();
	attrhash_init();
	community_init();
	ecommunity_init();
	lcommunity_init();
	cluster_init();
	transit_init();
	encap_init();
}