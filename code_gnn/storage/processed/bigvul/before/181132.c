  lookup_bytestring(netdissect_options *ndo, register const u_char *bs,
  		  const unsigned int nlen)
  {
	struct enamemem *tp;
// 	struct bsnamemem *tp;
  	register u_int i, j, k;
  
  	if (nlen >= 6) {
 		k = (bs[0] << 8) | bs[1];
 		j = (bs[2] << 8) | bs[3];
 		i = (bs[4] << 8) | bs[5];
 	} else if (nlen >= 4) {
 		k = (bs[0] << 8) | bs[1];
 		j = (bs[2] << 8) | bs[3];
 		i = 0;
 	} else
  		i = j = k = 0;
  
  	tp = &bytestringtable[(i ^ j) & (HASHNAMESIZE-1)];
	while (tp->e_nxt)
		if (tp->e_addr0 == i &&
		    tp->e_addr1 == j &&
		    tp->e_addr2 == k &&
		    memcmp((const char *)bs, (const char *)(tp->e_bs), nlen) == 0)
// 	while (tp->bs_nxt)
// 		if (nlen == tp->bs_nbytes &&
// 		    tp->bs_addr0 == i &&
// 		    tp->bs_addr1 == j &&
// 		    tp->bs_addr2 == k &&
// 		    memcmp((const char *)bs, (const char *)(tp->bs_bytes), nlen) == 0)
  			return tp;
  		else
			tp = tp->e_nxt;
// 			tp = tp->bs_nxt;
  
	tp->e_addr0 = i;
	tp->e_addr1 = j;
	tp->e_addr2 = k;
// 	tp->bs_addr0 = i;
// 	tp->bs_addr1 = j;
// 	tp->bs_addr2 = k;
  
	tp->e_bs = (u_char *) calloc(1, nlen + 1);
	if (tp->e_bs == NULL)
// 	tp->bs_bytes = (u_char *) calloc(1, nlen + 1);
// 	if (tp->bs_bytes == NULL)
  		(*ndo->ndo_error)(ndo, "lookup_bytestring: calloc");
  
	memcpy(tp->e_bs, bs, nlen);
	tp->e_nxt = (struct enamemem *)calloc(1, sizeof(*tp));
	if (tp->e_nxt == NULL)
// 	memcpy(tp->bs_bytes, bs, nlen);
// 	tp->bs_nbytes = nlen;
// 	tp->bs_nxt = (struct bsnamemem *)calloc(1, sizeof(*tp));
// 	if (tp->bs_nxt == NULL)
  		(*ndo->ndo_error)(ndo, "lookup_bytestring: calloc");
  
  	return tp;
 }