__ip_vs_service_get(int af, __u16 protocol, const union nf_inet_addr *vaddr,
		    __be16 vport)
{
	unsigned hash;
	struct ip_vs_service *svc;

	 
	hash = ip_vs_svc_hashkey(af, protocol, vaddr, vport);

	list_for_each_entry(svc, &ip_vs_svc_table[hash], s_list){
		if ((svc->af == af)
		    && ip_vs_addr_equal(af, &svc->addr, vaddr)
		    && (svc->port == vport)
		    && (svc->protocol == protocol)) {
			 
			atomic_inc(&svc->usecnt);
			return svc;
		}
	}

	return NULL;
}
