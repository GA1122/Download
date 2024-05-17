print_l2_routes(netdissect_options *ndo,
                const char *rp, u_int len)
{
	int count;
	int area;
	int info;

	 
	while (len > (3 * sizeof(short))) {
	    ND_TCHECK2(*rp, 3 * sizeof(short));
	    count = EXTRACT_LE_16BITS(rp);
	    if (count > 1024)
		return (1);	 
	    rp += sizeof(short);
	    len -= sizeof(short);
	    area = EXTRACT_LE_16BITS(rp);
	    rp += sizeof(short);
	    len -= sizeof(short);
	    info = EXTRACT_LE_16BITS(rp);
	    rp += sizeof(short);
	    len -= sizeof(short);
	    ND_PRINT((ndo, "{areas %d-%d cost %d hops %d} ", area, area + count,
			    RI_COST(info), RI_HOPS(info)));
	}
	return (1);

trunc:
	return (0);
}
