ip_vs_service_get(struct net *net, int af, __u32 fwmark, __u16 protocol,
		  const union nf_inet_addr *vaddr, __be16 vport)
{
	struct ip_vs_service *svc;
	struct netns_ipvs *ipvs = net_ipvs(net);

	read_lock(&__ip_vs_svc_lock);

	 
	if (fwmark) {
		svc = __ip_vs_svc_fwm_find(net, af, fwmark);
		if (svc)
			goto out;
	}

	 
	svc = __ip_vs_service_find(net, af, protocol, vaddr, vport);

	if (svc == NULL
	    && protocol == IPPROTO_TCP
	    && atomic_read(&ipvs->ftpsvc_counter)
	    && (vport == FTPDATA || ntohs(vport) >= PROT_SOCK)) {
		 
		svc = __ip_vs_service_find(net, af, protocol, vaddr, FTPPORT);
	}

	if (svc == NULL
	    && atomic_read(&ipvs->nullsvc_counter)) {
		 
		svc = __ip_vs_service_find(net, af, protocol, vaddr, 0);
	}

  out:
	if (svc)
		atomic_inc(&svc->usecnt);
	read_unlock(&__ip_vs_svc_lock);

	IP_VS_DBG_BUF(9, "lookup service: fwm %u %s %s:%u %s\n",
		      fwmark, ip_vs_proto_name(protocol),
		      IP_VS_DBG_ADDR(af, vaddr), ntohs(vport),
		      svc ? "hit" : "not hit");

	return svc;
}
