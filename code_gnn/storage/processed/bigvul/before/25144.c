static bool peer_pmtu_expired(struct inet_peer *peer)
{
	unsigned long orig = ACCESS_ONCE(peer->pmtu_expires);

	return orig &&
	       time_after_eq(jiffies, orig) &&
	       cmpxchg(&peer->pmtu_expires, orig, 0) == orig;
}
