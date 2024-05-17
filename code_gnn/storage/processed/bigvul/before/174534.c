make_message(struct dhcp_message **message,
 const struct interface *iface,
 uint8_t type)
{
 struct dhcp_message *dhcp;
 uint8_t *m, *lp, *p;
 uint8_t *n_params = NULL;
 time_t up = uptime() - iface->start_uptime;
 uint32_t ul;
 uint16_t sz;
 size_t len;
 const char *hp;
 const struct dhcp_opt *opt;
 const struct if_options *ifo = iface->state->options;
 const struct dhcp_lease *lease = &iface->state->lease;

	dhcp = xzalloc(sizeof (*dhcp));
	m = (uint8_t *)dhcp;
	p = dhcp->options;

 if ((type == DHCP_INFORM || type == DHCP_RELEASE ||
 (type == DHCP_REQUEST &&
		    iface->net.s_addr == lease->net.s_addr &&
 (iface->state->new == NULL ||
			iface->state->new->cookie == htonl(MAGIC_COOKIE)))))
 {
		dhcp->ciaddr = iface->addr.s_addr;
  
 if (type == DHCP_INFORM && iface->addr.s_addr == 0)
			dhcp->ciaddr = lease->addr.s_addr;
 }

	dhcp->op = DHCP_BOOTREQUEST;
	dhcp->hwtype = iface->family;
 switch (iface->family) {
 case ARPHRD_ETHER:
 case ARPHRD_IEEE802:
		dhcp->hwlen = iface->hwlen;
		memcpy(&dhcp->chaddr, &iface->hwaddr, iface->hwlen);
 break;
 }

 if (ifo->options & DHCPCD_BROADCAST &&
	    dhcp->ciaddr == 0 &&
	    type != DHCP_DECLINE &&
	    type != DHCP_RELEASE)
		dhcp->flags = htons(BROADCAST_FLAG);

 if (type != DHCP_DECLINE && type != DHCP_RELEASE) {
 if (up < 0 || up > (time_t)UINT16_MAX)
			dhcp->secs = htons((uint16_t)UINT16_MAX);
 else
			dhcp->secs = htons(up);
 }
	dhcp->xid = iface->state->xid;
	dhcp->cookie = htonl(MAGIC_COOKIE);

 *p++ = DHO_MESSAGETYPE; 
 *p++ = 1;
 *p++ = type;

 if (iface->clientid) {
 *p++ = DHO_CLIENTID;
		memcpy(p, iface->clientid, iface->clientid[0] + 1);
		p += iface->clientid[0] + 1;
 }

 if (lease->addr.s_addr && lease->cookie == htonl(MAGIC_COOKIE)) {
 if (type == DHCP_DECLINE ||
 (type == DHCP_REQUEST &&
			lease->addr.s_addr != iface->addr.s_addr))
 {
			PUTADDR(DHO_IPADDRESS, lease->addr);
 if (lease->server.s_addr)
				PUTADDR(DHO_SERVERID, lease->server);
 }

 if (type == DHCP_RELEASE) {
 if (lease->server.s_addr)
				PUTADDR(DHO_SERVERID, lease->server);
 }
 }

 if (type == DHCP_DECLINE) {
 *p++ = DHO_MESSAGE;
		len = strlen(DAD);
 *p++ = len;
		memcpy(p, DAD, len);
		p += len;
 }

 if (type == DHCP_DISCOVER && ifo->options & DHCPCD_REQUEST)
		PUTADDR(DHO_IPADDRESS, ifo->req_addr);

 if (type == DHCP_DISCOVER ||
	    type == DHCP_INFORM ||
	    type == DHCP_REQUEST)
 {
 *p++ = DHO_MAXMESSAGESIZE;
 *p++ = 2;
		sz = get_mtu(iface->name);
 if (sz < MTU_MIN) {
 if (set_mtu(iface->name, MTU_MIN) == 0)
				sz = MTU_MIN;
 } else if (sz > MTU_MAX) {
  
			sz = MTU_MAX;
 }
		sz = htons(sz);
		memcpy(p, &sz, 2);
		p += 2;

 if (ifo->userclass[0]) {
 *p++ = DHO_USERCLASS;
			memcpy(p, ifo->userclass, ifo->userclass[0] + 1);
			p += ifo->userclass[0] + 1;
 }

 if (ifo->vendorclassid[0]) {
 *p++ = DHO_VENDORCLASSID;
			memcpy(p, ifo->vendorclassid,
			    ifo->vendorclassid[0] + 1);
			p += ifo->vendorclassid[0] + 1;
 }


 if (type != DHCP_INFORM) {
 if (ifo->leasetime != 0) {
 *p++ = DHO_LEASETIME;
 *p++ = 4;
				ul = htonl(ifo->leasetime);
				memcpy(p, &ul, 4);
				p += 4;
 }
 }

  
 if (ifo->options & DHCPCD_HOSTNAME && ifo->hostname[0]) {
 *p++ = DHO_HOSTNAME;
			hp = strchr(ifo->hostname, '.');
 if (hp)
				len = hp - ifo->hostname;
 else
				len = strlen(ifo->hostname);
 *p++ = len;
			memcpy(p, ifo->hostname, len);
			p += len;
 }
 if (ifo->fqdn != FQDN_DISABLE && ifo->hostname[0]) {
  
 *p++ = DHO_FQDN;
			lp = p;
 *p++ = 3;
  
 *p++ = (ifo->fqdn & 0x09) | 0x04;
 *p++ = 0;  
 *p++ = 0;  
			ul = encode_rfc1035(ifo->hostname, p);
 *lp += ul;
			p += ul;
 }

  
 if (ifo->vendor[0]) {
 *p++ = DHO_VENDOR;
			memcpy(p, ifo->vendor, ifo->vendor[0] + 1);
			p += ifo->vendor[0] + 1;
 }

 *p++ = DHO_PARAMETERREQUESTLIST;
		n_params = p;
 *p++ = 0;
 for (opt = dhcp_opts; opt->option; opt++) {
 if (!(opt->type & REQUEST || 
				has_option_mask(ifo->requestmask, opt->option)))
 continue;
 if (type == DHCP_INFORM &&
 (opt->option == DHO_RENEWALTIME ||
				opt->option == DHO_REBINDTIME))
 continue;
 *p++ = opt->option;
 }
 *n_params = p - n_params - 1;
 }
 *p++ = DHO_END;

#ifdef BOOTP_MESSAGE_LENTH_MIN
  
 while (p - m < BOOTP_MESSAGE_LENTH_MIN)
 *p++ = DHO_PAD;
#endif

 *message = dhcp;
 return p - m;
}
