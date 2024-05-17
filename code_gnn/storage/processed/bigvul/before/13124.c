static struct dyn_lease *find_lease_by_mac(const uint8_t *mac)
{
	unsigned i;

	for (i = 0; i < server_config.max_leases; i++)
		if (memcmp(g_leases[i].lease_mac, mac, 6) == 0)
			return &g_leases[i];

	return NULL;
}
