unsigned short ip_rt_frag_needed(struct net *net, const struct iphdr *iph,
				 unsigned short new_mtu,
				 struct net_device *dev)
{
	unsigned short old_mtu = ntohs(iph->tot_len);
	unsigned short est_mtu = 0;
	struct inet_peer *peer;

	peer = inet_getpeer_v4(iph->daddr, 1);
	if (peer) {
		unsigned short mtu = new_mtu;

		if (new_mtu < 68 || new_mtu >= old_mtu) {
			 
			if (mtu == 0 &&
			    old_mtu >= 68 + (iph->ihl << 2))
				old_mtu -= iph->ihl << 2;
			mtu = guess_mtu(old_mtu);
		}

		if (mtu < ip_rt_min_pmtu)
			mtu = ip_rt_min_pmtu;
		if (!peer->pmtu_expires || mtu < peer->pmtu_learned) {
			unsigned long pmtu_expires;

			pmtu_expires = jiffies + ip_rt_mtu_expires;
			if (!pmtu_expires)
				pmtu_expires = 1UL;

			est_mtu = mtu;
			peer->pmtu_learned = mtu;
			peer->pmtu_expires = pmtu_expires;
		}

		inet_putpeer(peer);

		atomic_inc(&__rt_peer_genid);
	}
	return est_mtu ? : new_mtu;
}
