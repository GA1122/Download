static void check_network(Bridge *br) {
	assert(br);
	if (br->macvlan == 0)  
		net_configure_sandbox_ip(br);
	else if (br->ipsandbox) {  
		char *rv = in_netrange(br->ipsandbox, br->ip, br->mask);
		if (rv) {
			fprintf(stderr, "%s", rv);
			exit(1);
		}
	}
}
