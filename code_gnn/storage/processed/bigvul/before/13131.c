static int nobody_responds_to_arp(uint32_t nip, const uint8_t *safe_mac, unsigned arpping_ms)
{
	struct in_addr temp;
	int r;

	r = arpping(nip, safe_mac,
			server_config.server_nip,
			server_config.server_mac,
			server_config.interface,
			arpping_ms);
	if (r)
		return r;

	temp.s_addr = nip;
	bb_error_msg("%s belongs to someone, reserving it for %u seconds",
		inet_ntoa(temp), (unsigned)server_config.conflict_time);
	add_lease(NULL, nip, server_config.conflict_time, NULL, 0);
	return 0;
}
