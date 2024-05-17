static struct dyn_lease *oldest_expired_lease(void)
{
	struct dyn_lease *oldest_lease = NULL;
	leasetime_t oldest_time = time(NULL);
	unsigned i;

	 
	for (i = 0; i < server_config.max_leases; i++) {
		if (g_leases[i].expires == 0  
		 || g_leases[i].expires < oldest_time
		) {
			oldest_time = g_leases[i].expires;
			oldest_lease = &g_leases[i];
		}
	}
	return oldest_lease;
}
