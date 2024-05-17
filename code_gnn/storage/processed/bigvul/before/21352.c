unsigned slab_node(struct mempolicy *policy)
{
	if (!policy || policy->flags & MPOL_F_LOCAL)
		return numa_node_id();

	switch (policy->mode) {
	case MPOL_PREFERRED:
		 
		return policy->v.preferred_node;

	case MPOL_INTERLEAVE:
		return interleave_nodes(policy);

	case MPOL_BIND: {
		 
		struct zonelist *zonelist;
		struct zone *zone;
		enum zone_type highest_zoneidx = gfp_zone(GFP_KERNEL);
		zonelist = &NODE_DATA(numa_node_id())->node_zonelists[0];
		(void)first_zones_zonelist(zonelist, highest_zoneidx,
							&policy->v.nodes,
							&zone);
		return zone ? zone->node : numa_node_id();
	}

	default:
		BUG();
	}
}
