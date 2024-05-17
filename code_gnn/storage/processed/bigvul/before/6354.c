routes_are_duplicate (const NMPlatformIP4Route *a, const NMPlatformIP4Route *b, gboolean consider_gateway_and_metric)
{
	return a->network == b->network && a->plen == b->plen &&
	       (!consider_gateway_and_metric || (a->gateway == b->gateway && a->metric == b->metric));
}
