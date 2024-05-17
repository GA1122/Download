evutil_check_ifaddrs(void)
{
#if defined(EVENT__HAVE_GETIFADDRS)
	 
	struct ifaddrs *ifa = NULL;
	const struct ifaddrs *i;
	if (getifaddrs(&ifa) < 0) {
		event_warn("Unable to call getifaddrs()");
		return -1;
	}

	for (i = ifa; i; i = i->ifa_next) {
		if (!i->ifa_addr)
			continue;
		evutil_found_ifaddr(i->ifa_addr);
	}

	freeifaddrs(ifa);
	return 0;
#elif defined(_WIN32)
	 
	HMODULE lib = evutil_load_windows_system_library_(TEXT("ihplapi.dll"));
	GetAdaptersAddresses_fn_t fn;
	ULONG size, res;
	IP_ADAPTER_ADDRESSES *addresses = NULL, *address;
	int result = -1;

#define FLAGS (GAA_FLAG_SKIP_ANYCAST | \
               GAA_FLAG_SKIP_MULTICAST | \
               GAA_FLAG_SKIP_DNS_SERVER)

	if (!lib)
		goto done;

	if (!(fn = (GetAdaptersAddresses_fn_t) GetProcAddress(lib, "GetAdaptersAddresses")))
		goto done;

	 
	size = 15*1024;
	addresses = mm_malloc(size);
	if (!addresses)
		goto done;
	res = fn(AF_UNSPEC, FLAGS, NULL, addresses, &size);
	if (res == ERROR_BUFFER_OVERFLOW) {
		 
		mm_free(addresses);
		addresses = mm_malloc(size);
		if (!addresses)
			goto done;
		res = fn(AF_UNSPEC, FLAGS, NULL, addresses, &size);
	}
	if (res != NO_ERROR)
		goto done;

	for (address = addresses; address; address = address->Next) {
		IP_ADAPTER_UNICAST_ADDRESS *a;
		for (a = address->FirstUnicastAddress; a; a = a->Next) {
			 
			struct sockaddr *sa = a->Address.lpSockaddr;
			evutil_found_ifaddr(sa);
		}
	}

	result = 0;
done:
	if (lib)
		FreeLibrary(lib);
	if (addresses)
		mm_free(addresses);
	return result;
#else
	return -1;
#endif
}
