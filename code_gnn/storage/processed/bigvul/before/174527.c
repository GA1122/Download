get_lease(struct dhcp_lease *lease, const struct dhcp_message *dhcp)
{
 struct timeval now;

	lease->cookie = dhcp->cookie;
  
 if (dhcp->yiaddr)
		lease->addr.s_addr = dhcp->yiaddr;
 else
		lease->addr.s_addr = dhcp->ciaddr;
 if (get_option_addr(&lease->net, dhcp, DHO_SUBNETMASK) == -1)
		lease->net.s_addr = get_netmask(lease->addr.s_addr);
 if (get_option_addr(&lease->brd, dhcp, DHO_BROADCAST) == -1)
		lease->brd.s_addr = lease->addr.s_addr | ~lease->net.s_addr;
 if (get_option_uint32(&lease->leasetime, dhcp, DHO_LEASETIME) == 0) {
  
		get_monotonic(&now);
 if (now.tv_sec + (time_t)lease->leasetime < now.tv_sec)
			lease->leasetime = ~0U;  
 } else
		lease->leasetime = ~0U;  
 if (get_option_uint32(&lease->renewaltime, dhcp, DHO_RENEWALTIME) != 0)
		lease->renewaltime = 0;
 if (get_option_uint32(&lease->rebindtime, dhcp, DHO_REBINDTIME) != 0)
		lease->rebindtime = 0;
 if (get_option_addr(&lease->server, dhcp, DHO_SERVERID) != 0)
		lease->server.s_addr = INADDR_ANY;
}
