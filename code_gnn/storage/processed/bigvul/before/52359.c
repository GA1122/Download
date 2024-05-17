ip6_packet_match(const struct sk_buff *skb,
		 const char *indev,
		 const char *outdev,
		 const struct ip6t_ip6 *ip6info,
		 unsigned int *protoff,
		 int *fragoff, bool *hotdrop)
{
	unsigned long ret;
	const struct ipv6hdr *ipv6 = ipv6_hdr(skb);

#define FWINV(bool, invflg) ((bool) ^ !!(ip6info->invflags & (invflg)))

	if (FWINV(ipv6_masked_addr_cmp(&ipv6->saddr, &ip6info->smsk,
				       &ip6info->src), IP6T_INV_SRCIP) ||
	    FWINV(ipv6_masked_addr_cmp(&ipv6->daddr, &ip6info->dmsk,
				       &ip6info->dst), IP6T_INV_DSTIP)) {
		dprintf("Source or dest mismatch.\n");
 
		return false;
	}

	ret = ifname_compare_aligned(indev, ip6info->iniface, ip6info->iniface_mask);

	if (FWINV(ret != 0, IP6T_INV_VIA_IN)) {
		dprintf("VIA in mismatch (%s vs %s).%s\n",
			indev, ip6info->iniface,
			ip6info->invflags & IP6T_INV_VIA_IN ? " (INV)" : "");
		return false;
	}

	ret = ifname_compare_aligned(outdev, ip6info->outiface, ip6info->outiface_mask);

	if (FWINV(ret != 0, IP6T_INV_VIA_OUT)) {
		dprintf("VIA out mismatch (%s vs %s).%s\n",
			outdev, ip6info->outiface,
			ip6info->invflags & IP6T_INV_VIA_OUT ? " (INV)" : "");
		return false;
	}

 

	 
	if (ip6info->flags & IP6T_F_PROTO) {
		int protohdr;
		unsigned short _frag_off;

		protohdr = ipv6_find_hdr(skb, protoff, -1, &_frag_off, NULL);
		if (protohdr < 0) {
			if (_frag_off == 0)
				*hotdrop = true;
			return false;
		}
		*fragoff = _frag_off;

		dprintf("Packet protocol %hi ?= %s%hi.\n",
				protohdr,
				ip6info->invflags & IP6T_INV_PROTO ? "!":"",
				ip6info->proto);

		if (ip6info->proto == protohdr) {
			if (ip6info->invflags & IP6T_INV_PROTO)
				return false;

			return true;
		}

		 
		if ((ip6info->proto != 0) &&
			!(ip6info->invflags & IP6T_INV_PROTO))
			return false;
	}
	return true;
}