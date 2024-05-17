static struct dyn_lease *add_lease(
		const uint8_t *chaddr, uint32_t yiaddr,
		leasetime_t leasetime,
		const char *hostname, int hostname_len)
{
	struct dyn_lease *oldest;

	 
	clear_leases(chaddr, yiaddr);

	oldest = oldest_expired_lease();

	if (oldest) {
		memset(oldest, 0, sizeof(*oldest));
		if (hostname) {
			char *p;

			hostname_len++;  
			if (hostname_len > sizeof(oldest->hostname))
				hostname_len = sizeof(oldest->hostname);
			p = safe_strncpy(oldest->hostname, hostname, hostname_len);
			 
			while (*p) {
				if (!isalnum(*p) && *p != '-' && *p != '_')
					*p = '.';
				p++;
			}
		}
		if (chaddr)
			memcpy(oldest->lease_mac, chaddr, 6);
		oldest->lease_nip = yiaddr;
		oldest->expires = time(NULL) + leasetime;
	}

	return oldest;
}
