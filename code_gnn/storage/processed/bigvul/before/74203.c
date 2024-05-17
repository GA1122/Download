get_next_address(
	address_node *addr
	)
{
	const char addr_prefix[] = "192.168.0.";
	static int curr_addr_num = 1;
#define ADDR_LENGTH 16 + 1	 
	char addr_string[ADDR_LENGTH];
	sockaddr_u *final_addr;
	struct addrinfo *ptr;
	int gai_error;
	
	final_addr = emalloc(sizeof(*final_addr));

	if (addr->type == T_String) {
		snprintf(addr_string, sizeof(addr_string), "%s%d",
			 addr_prefix, curr_addr_num++);
		printf("Selecting ip address %s for hostname %s\n",
		       addr_string, addr->address);
		gai_error = getaddrinfo(addr_string, "ntp", NULL, &ptr);
	} else {
		gai_error = getaddrinfo(addr->address, "ntp", NULL, &ptr);
	}

	if (gai_error) {
		fprintf(stderr, "ERROR!! Could not get a new address\n");
		exit(1);
	}
	memcpy(final_addr, ptr->ai_addr, ptr->ai_addrlen);
	fprintf(stderr, "Successful in setting ip address of simulated server to: %s\n",
		stoa(final_addr));
	freeaddrinfo(ptr);

	return final_addr;
}