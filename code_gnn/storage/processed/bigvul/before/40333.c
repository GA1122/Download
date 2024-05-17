static struct atalk_route *atrtr_find(struct atalk_addr *target)
{
	 
	struct atalk_route *net_route = NULL;
	struct atalk_route *r;

	read_lock_bh(&atalk_routes_lock);
	for (r = atalk_routes; r; r = r->next) {
		if (!(r->flags & RTF_UP))
			continue;

		if (r->target.s_net == target->s_net) {
			if (r->flags & RTF_HOST) {
				 
				if (r->target.s_node == target->s_node)
					goto out;
			} else
				 
				net_route = r;
		}
	}

	 
	if (net_route)
		r = net_route;
	else if (atrtr_default.dev)
		r = &atrtr_default;
	else  
		r = NULL;
out:
	read_unlock_bh(&atalk_routes_lock);
	return r;
}
