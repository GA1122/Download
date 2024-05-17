static inline unsigned int dn_current_mss(struct sock *sk, int flags)
{
	struct dst_entry *dst = __sk_dst_get(sk);
	struct dn_scp *scp = DN_SK(sk);
	int mss_now = min_t(int, scp->segsize_loc, scp->segsize_rem);

	 
	if (flags & MSG_OOB)
		return 16;

	 
	if (dst) {
		u32 mtu = dst_mtu(dst);
		mss_now = min_t(int, dn_mss_from_pmtu(dst->dev, mtu), mss_now);
	}

	return mss_now;
}
