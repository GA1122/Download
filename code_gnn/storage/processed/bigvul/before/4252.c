PHP_FUNCTION(ip2long)
{
	char *addr;
	int addr_len;
#ifdef HAVE_INET_PTON
	struct in_addr ip;
#else
	unsigned long int ip;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &addr, &addr_len) == FAILURE) {
		return;
	}

#ifdef HAVE_INET_PTON
	if (addr_len == 0 || inet_pton(AF_INET, addr, &ip) != 1) {
		RETURN_FALSE;
	}
	RETURN_LONG(ntohl(ip.s_addr));
#else
	if (addr_len == 0 || (ip = inet_addr(addr)) == INADDR_NONE) {
		 
		if (addr_len == sizeof("255.255.255.255") - 1 &&
			!memcmp(addr, "255.255.255.255", sizeof("255.255.255.255") - 1)
		) {
			RETURN_LONG(0xFFFFFFFF);
		}
		RETURN_FALSE;
	}
	RETURN_LONG(ntohl(ip));
#endif
}
