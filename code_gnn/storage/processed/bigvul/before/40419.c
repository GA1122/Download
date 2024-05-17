__be16 ipx_cksum(struct ipxhdr *packet, int length)
{
	 
	 
	__u16 *p = (__u16 *)packet;
	__u32 sum = p[1] + (p[2] & (__force u16)htons(0x00ff));
	__u32 i = (length >> 1) - 3;  

	 
	p += 3;
	while (i--)
		sum += *p++;

	 
	if (packet->ipx_pktsize & htons(1))
		sum += (__force u16)htons(0xff00) & *p;

	 
	sum = (sum & 0xffff) + (sum >> 16);

	 
	if (sum >= 0x10000)
		sum++;

	 
	if (sum)
		sum = ~sum;

	return (__force __be16)sum;
}
