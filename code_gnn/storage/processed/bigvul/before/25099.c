static void check_peer_pmtu(struct dst_entry *dst, struct inet_peer *peer)
{
	unsigned long expires = ACCESS_ONCE(peer->pmtu_expires);

	if (!expires)
		return;
	if (time_before(jiffies, expires)) {
		u32 orig_dst_mtu = dst_mtu(dst);
		if (peer->pmtu_learned < orig_dst_mtu) {
			if (!peer->pmtu_orig)
				peer->pmtu_orig = dst_metric_raw(dst, RTAX_MTU);
			dst_metric_set(dst, RTAX_MTU, peer->pmtu_learned);
		}
	} else if (cmpxchg(&peer->pmtu_expires, expires, 0) == expires)
		dst_metric_set(dst, RTAX_MTU, peer->pmtu_orig);
}
