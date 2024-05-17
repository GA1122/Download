int mpol_to_str(char *buffer, int maxlen, struct mempolicy *pol, int no_context)
{
	char *p = buffer;
	int l;
	nodemask_t nodes;
	unsigned short mode;
	unsigned short flags = pol ? pol->flags : 0;

	 
	VM_BUG_ON(maxlen < strlen("interleave") + strlen("relative") + 16);

	if (!pol || pol == &default_policy)
		mode = MPOL_DEFAULT;
	else
		mode = pol->mode;

	switch (mode) {
	case MPOL_DEFAULT:
		nodes_clear(nodes);
		break;

	case MPOL_PREFERRED:
		nodes_clear(nodes);
		if (flags & MPOL_F_LOCAL)
			mode = MPOL_LOCAL;	 
		else
			node_set(pol->v.preferred_node, nodes);
		break;

	case MPOL_BIND:
		 
	case MPOL_INTERLEAVE:
		if (no_context)
			nodes = pol->w.user_nodemask;
		else
			nodes = pol->v.nodes;
		break;

	default:
		BUG();
	}

	l = strlen(policy_modes[mode]);
	if (buffer + maxlen < p + l + 1)
		return -ENOSPC;

	strcpy(p, policy_modes[mode]);
	p += l;

	if (flags & MPOL_MODE_FLAGS) {
		if (buffer + maxlen < p + 2)
			return -ENOSPC;
		*p++ = '=';

		 
		if (flags & MPOL_F_STATIC_NODES)
			p += snprintf(p, buffer + maxlen - p, "static");
		else if (flags & MPOL_F_RELATIVE_NODES)
			p += snprintf(p, buffer + maxlen - p, "relative");
	}

	if (!nodes_empty(nodes)) {
		if (buffer + maxlen < p + 2)
			return -ENOSPC;
		*p++ = ':';
	 	p += nodelist_scnprintf(p, buffer + maxlen - p, nodes);
	}
	return p - buffer;
}
