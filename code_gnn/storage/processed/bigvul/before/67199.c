static nodemask_t *policy_nodemask(gfp_t gfp, struct mempolicy *policy)
{
	 
	if (unlikely(policy->mode == MPOL_BIND) &&
			apply_policy_zone(policy, gfp_zone(gfp)) &&
			cpuset_nodemask_valid_mems_allowed(&policy->v.nodes))
		return &policy->v.nodes;

	return NULL;
}