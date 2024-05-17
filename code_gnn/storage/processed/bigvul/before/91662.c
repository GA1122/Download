size_t bgp_packet_mpunreach_start(struct stream *s, afi_t afi, safi_t safi)
{
	unsigned long attrlen_pnt;
	iana_afi_t pkt_afi;
	iana_safi_t pkt_safi;

	 
	stream_putc(s, BGP_ATTR_FLAG_OPTIONAL | BGP_ATTR_FLAG_EXTLEN);
	stream_putc(s, BGP_ATTR_MP_UNREACH_NLRI);

	attrlen_pnt = stream_get_endp(s);
	stream_putw(s, 0);  

	 
	bgp_map_afi_safi_int2iana(afi, safi, &pkt_afi, &pkt_safi);

	stream_putw(s, pkt_afi);
	stream_putc(s, pkt_safi);

	return attrlen_pnt;
}