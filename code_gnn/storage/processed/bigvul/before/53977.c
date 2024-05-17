enum ndp_route_preference ndp_msgra_route_preference(struct ndp_msgra *msgra)
{
	uint8_t prf = (msgra->ra->nd_ra_flags_reserved >> 3) & 3;

	 
	if (prf == 2 || !ndp_msgra_router_lifetime(msgra))
		prf = 0;
	return prf;
}
