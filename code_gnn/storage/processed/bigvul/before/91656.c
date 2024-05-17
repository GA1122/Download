void bgp_packet_mpattr_end(struct stream *s, size_t sizep)
{
	 
	stream_putw_at(s, sizep, (stream_get_endp(s) - sizep) - 2);
}
