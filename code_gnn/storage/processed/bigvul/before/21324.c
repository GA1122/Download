static int mpol_new_bind(struct mempolicy *pol, const nodemask_t *nodes)
{
	if (!is_valid_nodemask(nodes))
		return -EINVAL;
	pol->v.nodes = *nodes;
	return 0;
}
