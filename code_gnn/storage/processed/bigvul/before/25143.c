static bool peer_pmtu_cleaned(struct inet_peer *peer)
{
	unsigned long orig = ACCESS_ONCE(peer->pmtu_expires);

	return orig &&
	       cmpxchg(&peer->pmtu_expires, orig, 0) == orig;
}
