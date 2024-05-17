static uint32_t find_free_or_expired_nip(const uint8_t *safe_mac, unsigned arpping_ms)
{
	uint32_t addr;
	struct dyn_lease *oldest_lease = NULL;

#if ENABLE_FEATURE_UDHCPD_BASE_IP_ON_MAC
	uint32_t stop;
	unsigned i, hash;

	 
	hash = 0;
	for (i = 0; i < 6; i++)
		hash += safe_mac[i] + (hash << 6) + (hash << 16) - hash;

	 
	addr = server_config.start_ip
		+ (hash % (1 + server_config.end_ip - server_config.start_ip));
	stop = addr;
#else
	addr = server_config.start_ip;
#define stop (server_config.end_ip + 1)
#endif
	do {
		uint32_t nip;
		struct dyn_lease *lease;

		 
		if ((addr & 0xff) == 0)
			goto next_addr;
		 
		if ((addr & 0xff) == 0xff)
			goto next_addr;
		nip = htonl(addr);
		 
		if (nip == server_config.server_nip)
			goto next_addr;
		 
		if (is_nip_reserved(server_config.static_leases, nip))
			goto next_addr;

		lease = find_lease_by_nip(nip);
		if (!lease) {
			if (nobody_responds_to_arp(nip, safe_mac, arpping_ms))
				return nip;
		} else {
			if (!oldest_lease || lease->expires < oldest_lease->expires)
				oldest_lease = lease;
		}

 next_addr:
		addr++;
#if ENABLE_FEATURE_UDHCPD_BASE_IP_ON_MAC
		if (addr > server_config.end_ip)
			addr = server_config.start_ip;
#endif
	} while (addr != stop);

	if (oldest_lease
	 && is_expired_lease(oldest_lease)
	 && nobody_responds_to_arp(oldest_lease->lease_nip, safe_mac, arpping_ms)
	) {
		return oldest_lease->lease_nip;
	}

	return 0;
}
