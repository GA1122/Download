static nodemask_t *policy_nodemask(gfp_t gfp, struct mempolicy *policy)
{
	 
	if (unlikely(policy->mode == MPOL_BIND) &&
			gfp_zone(gfp) >= policy_zone &&
			cpuset_nodemask_valid_mems_allowed(&policy->v.nodes))
		return &policy->v.nodes;

	return NULL;
}
