static int dns_build_query(int query_id, int query_type, unsigned int accepted_payload_size,
			   char *hostname_dn, int hostname_dn_len, char *buf, int bufsize)
{
	struct dns_header            dns_hdr;
	struct dns_question          qinfo;
	struct dns_additional_record edns;
	char *p = buf;

	if (sizeof(dns_hdr) + sizeof(qinfo) +  sizeof(edns) + hostname_dn_len >= bufsize)
		return -1;

	memset(buf, 0, bufsize);

	 
	dns_hdr.id      = (unsigned short) htons(query_id);
	dns_hdr.flags   = htons(0x0100);  
	dns_hdr.qdcount = htons(1);       
	dns_hdr.ancount = 0;
	dns_hdr.nscount = 0;
	dns_hdr.arcount = htons(1);
	memcpy(p, &dns_hdr, sizeof(dns_hdr));
	p += sizeof(dns_hdr);

	 
	memcpy(p, hostname_dn, hostname_dn_len);
	p += hostname_dn_len;
	*p++ = 0;

	 
	qinfo.qtype  = htons(query_type);
	qinfo.qclass = htons(DNS_RCLASS_IN);
	memcpy(p, &qinfo, sizeof(qinfo));
	p += sizeof(qinfo);

	 
	edns.name             = 0;
	edns.type             = htons(DNS_RTYPE_OPT);
	edns.udp_payload_size = htons(accepted_payload_size);
	edns.extension        = 0;
	edns.data_length      = 0;
	memcpy(p, &edns, sizeof(edns));
	p += sizeof(edns);

	return (p - buf);
}
