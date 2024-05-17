static int macvlan_addr_busy(const struct macvlan_port *port,
				const unsigned char *addr)
{
	 
	if (!compare_ether_addr_64bits(port->dev->dev_addr, addr))
		return 1;

	if (macvlan_hash_lookup(port, addr))
		return 1;

	return 0;
}
