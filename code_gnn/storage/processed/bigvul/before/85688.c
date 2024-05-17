static unsigned int hns_nic_get_headlen(unsigned char *data, u32 flag,
					unsigned int max_size)
{
	unsigned char *network;
	u8 hlen;

	 
	if (max_size < ETH_HLEN)
		return max_size;

	 
	network = data;

	 
	network += ETH_HLEN;

	 
	if (hnae_get_field(flag, HNS_RXD_VLAN_M, HNS_RXD_VLAN_S)
		== HNS_RX_FLAG_VLAN_PRESENT) {
		if ((typeof(max_size))(network - data) > (max_size - VLAN_HLEN))
			return max_size;

		network += VLAN_HLEN;
	}

	 
	if (hnae_get_field(flag, HNS_RXD_L3ID_M, HNS_RXD_L3ID_S)
		== HNS_RX_FLAG_L3ID_IPV4) {
		if ((typeof(max_size))(network - data) >
		    (max_size - sizeof(struct iphdr)))
			return max_size;

		 
		hlen = (network[0] & 0x0F) << 2;

		 
		if (hlen < sizeof(struct iphdr))
			return network - data;

		 
	} else if (hnae_get_field(flag, HNS_RXD_L3ID_M, HNS_RXD_L3ID_S)
		== HNS_RX_FLAG_L3ID_IPV6) {
		if ((typeof(max_size))(network - data) >
		    (max_size - sizeof(struct ipv6hdr)))
			return max_size;

		 
		hlen = sizeof(struct ipv6hdr);
	} else {
		return network - data;
	}

	 
	network += hlen;

	 
	if (hnae_get_field(flag, HNS_RXD_L4ID_M, HNS_RXD_L4ID_S)
		== HNS_RX_FLAG_L4ID_TCP) {
		if ((typeof(max_size))(network - data) >
		    (max_size - sizeof(struct tcphdr)))
			return max_size;

		 
		hlen = (network[12] & 0xF0) >> 2;

		 
		if (hlen < sizeof(struct tcphdr))
			return network - data;

		network += hlen;
	} else if (hnae_get_field(flag, HNS_RXD_L4ID_M, HNS_RXD_L4ID_S)
		== HNS_RX_FLAG_L4ID_UDP) {
		if ((typeof(max_size))(network - data) >
		    (max_size - sizeof(struct udphdr)))
			return max_size;

		network += sizeof(struct udphdr);
	}

	 
	if ((typeof(max_size))(network - data) < max_size)
		return network - data;
	else
		return max_size;
}
