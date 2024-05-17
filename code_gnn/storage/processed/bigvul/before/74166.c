create_unpeer_node(
	address_node *addr
	)
{
	unpeer_node *	my_node;
	u_int		u;
	char *		pch;

	my_node = emalloc_zero(sizeof(*my_node));

	 
	pch = addr->address;
	while (*pch && isdigit(*pch))
		pch++;

	if (!*pch 
	    && 1 == sscanf(addr->address, "%u", &u)
	    && u <= ASSOCID_MAX) {
		my_node->assocID = (associd_t)u;
		destroy_address_node(addr);
		my_node->addr = NULL;
	} else {
		my_node->assocID = 0;
		my_node->addr = addr;
	}

	return my_node;
}
