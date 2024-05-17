get_option_routes(const struct dhcp_message *dhcp,
 const char *ifname, unsigned long long *opts)
{
 const uint8_t *p;
 const uint8_t *e;
 struct rt *routes = NULL;
 struct rt *route = NULL;
 int len;

  
	p = get_option(dhcp, DHO_CSR, &len, NULL);
  
 if (!p)
		p = get_option(dhcp, DHO_MSCSR, &len, NULL);
 if (p) {
		routes = decode_rfc3442_rt(len, p);
 if (routes) {
 if (!(*opts & DHCPCD_CSR_WARNED)) {
				syslog(LOG_DEBUG,
 "%s: using Classless Static Routes",
				    ifname);
 *opts |= DHCPCD_CSR_WARNED;
 }
 return routes;
 }
 }

  
	p = get_option(dhcp, DHO_STATICROUTE, &len, NULL);
 if (p) {
		e = p + len;
 while (p < e) {
 if (route) {
				route->next = xmalloc(sizeof(*route));
				route = route->next;
 } else
				routes = route = xmalloc(sizeof(*routes));
			route->next = NULL;
			memcpy(&route->dest.s_addr, p, 4);
			p += 4;
			memcpy(&route->gate.s_addr, p, 4);
			p += 4;
			route->net.s_addr = route_netmask(route->dest.s_addr);
 }
 }

  
	p = get_option(dhcp, DHO_ROUTER, &len, NULL);
 if (p) {
		e = p + len;
 while (p < e) {
 if (route) {
				route->next = xzalloc(sizeof(*route));
				route = route->next;
 } else
				routes = route = xzalloc(sizeof(*route));
			memcpy(&route->gate.s_addr, p, 4);
			p += 4;
 }
 }

 return routes;
}
