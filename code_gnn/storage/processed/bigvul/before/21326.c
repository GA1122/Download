static int mpol_new_preferred(struct mempolicy *pol, const nodemask_t *nodes)
{
	if (!nodes)
		pol->flags |= MPOL_F_LOCAL;	 
	else if (nodes_empty(*nodes))
		return -EINVAL;			 
	else
		pol->v.preferred_node = first_node(*nodes);
	return 0;
}
