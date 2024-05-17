static int ssdp_init(int in, char *iflist[], size_t num)
{
	int modified;
	size_t i;
	struct ifaddrs *ifaddrs, *ifa;

	logit(LOG_INFO, "Updating interfaces ...");

	if (getifaddrs(&ifaddrs) < 0) {
		logit(LOG_ERR, "Failed getifaddrs(): %s", strerror(errno));
		return -1;
	}

	 
	mark();

	 
	for (ifa = ifaddrs; ifa; ifa = ifa->ifa_next) {
		struct ifsock *ifs;

		 
		ifs = find_iface(ifa->ifa_addr);
		if (ifs) {
			ifs->stale = 0;
			continue;
		}
	}

	 
	modified = sweep();

	 
	for (ifa = ifaddrs; ifa; ifa = ifa->ifa_next) {
		int sd;

		 
		if (filter_iface(ifa->ifa_name, iflist, num)) {
			logit(LOG_DEBUG, "Skipping %s, not in iflist.", ifa->ifa_name);
			continue;
		}

		 
		if (filter_addr(ifa->ifa_addr))
			continue;

		sd = open_socket(ifa->ifa_name, ifa->ifa_addr, MC_SSDP_PORT);
		if (sd < 0)
			continue;

		multicast_join(in, ifa->ifa_addr);

		if (register_socket(in, sd, ifa->ifa_addr, ifa->ifa_netmask, ssdp_recv)) {
			close(sd);
			break;
		}
		modified++;
	}

	freeifaddrs(ifaddrs);

	return modified;
}
