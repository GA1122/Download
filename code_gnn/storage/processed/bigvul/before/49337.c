static unsigned fwnet_max_payload(unsigned max_rec, unsigned speed)
{
	max_rec = min(max_rec, speed + 8);
	max_rec = clamp(max_rec, 8U, 11U);  

	return (1 << (max_rec + 1)) - RFC2374_FRAG_HDR_SIZE;
}