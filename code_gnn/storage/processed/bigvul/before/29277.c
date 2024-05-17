ip_vs_lookup_dest(struct ip_vs_service *svc, const union nf_inet_addr *daddr,
		  __be16 dport)
{
	struct ip_vs_dest *dest;

	 
	list_for_each_entry(dest, &svc->destinations, n_list) {
		if ((dest->af == svc->af)
		    && ip_vs_addr_equal(svc->af, &dest->addr, daddr)
		    && (dest->port == dport)) {
			 
			return dest;
		}
	}

	return NULL;
}
