static void sandbox_if_up(Bridge *br) {
	assert(br);
	if (!br->configured)
		return;

	char *dev = br->devsandbox;
	net_if_up(dev);

	if (br->arg_ip_none == 1);	 
	else if (br->arg_ip_none == 0 && br->macvlan == 0) {
		if (br->ipsandbox == br->ip) {
			fprintf(stderr, "Error: %d.%d.%d.%d is interface %s address.\n", PRINT_IP(br->ipsandbox), br->dev);
			exit(1);
		}

		assert(br->ipsandbox);
		if (arg_debug)
			printf("Configuring %d.%d.%d.%d address on interface %s\n", PRINT_IP(br->ipsandbox), dev);
		net_config_interface(dev, br->ipsandbox, br->mask, br->mtu);
		arp_announce(dev, br);
	}
	else if (br->arg_ip_none == 0 && br->macvlan == 1) {
		if (br->ipsandbox == 0)
			br->ipsandbox = arp_assign(dev, br);  
		else {
			if (br->ipsandbox == br->ip) {
				fprintf(stderr, "Error: %d.%d.%d.%d is interface %s address.\n", PRINT_IP(br->ipsandbox), br->dev);
				exit(1);
			}

			uint32_t rv = arp_check(dev, br->ipsandbox);
			if (rv) {
				fprintf(stderr, "Error: the address %d.%d.%d.%d is already in use.\n", PRINT_IP(br->ipsandbox));
				exit(1);
			}
		}

		if (arg_debug)
			printf("Configuring %d.%d.%d.%d address on interface %s\n", PRINT_IP(br->ipsandbox), dev);
		net_config_interface(dev, br->ipsandbox, br->mask, br->mtu);
		arp_announce(dev, br);
	}

	if (br->ip6sandbox)
		 net_if_ip6(dev, br->ip6sandbox);
}