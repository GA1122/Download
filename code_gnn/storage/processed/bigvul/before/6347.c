addresses_are_duplicate (const NMPlatformIP4Address *a, const NMPlatformIP4Address *b, gboolean consider_plen)
{
	return a->address == b->address && (!consider_plen || a->plen == b->plen);
}
