static int find_node_from_desc(const void *fit, int node, const char *str)
{
	int child;

	if (node < 0)
		return -EINVAL;

	 
	for (child = fdt_first_subnode(fit, node); child >= 0;
	     child = fdt_next_subnode(fit, child)) {
		int len;
		const char *desc = fdt_getprop(fit, child, "description", &len);

		if (!desc)
			continue;

		if (!strcmp(desc, str))
			return child;
	}

	return -ENOENT;
}
