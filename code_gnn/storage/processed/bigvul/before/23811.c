static void macvlan_hash_change_addr(struct macvlan_dev *vlan,
					const unsigned char *addr)
{
	macvlan_hash_del(vlan, true);
	 
	memcpy(vlan->dev->dev_addr, addr, ETH_ALEN);
	macvlan_hash_add(vlan);
}
