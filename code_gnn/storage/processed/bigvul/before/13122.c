static void clear_leases(const uint8_t *chaddr, uint32_t yiaddr)
{
	unsigned i;

	for (i = 0; i < server_config.max_leases; i++) {
		if ((chaddr && memcmp(g_leases[i].lease_mac, chaddr, 6) == 0)
		 || (yiaddr && g_leases[i].lease_nip == yiaddr)
		) {
			memset(&g_leases[i], 0, sizeof(g_leases[i]));
		}
	}
}
