 ieee802_11_print(netdissect_options *ndo,
                  const u_char *p, u_int length, u_int orig_caplen, int pad,
                  u_int fcslen)
 {
 	uint16_t fc;
 	u_int caplen, hdrlen, meshdrlen;
 	struct lladdr_info src, dst;
 	int llc_hdrlen;
 
 	caplen = orig_caplen;
 	 
 	if (length < fcslen) {
 		ND_PRINT((ndo, "%s", tstr));
 		return caplen;
 	}
 	length -= fcslen;
 	if (caplen > length) {
 		 
 		fcslen = caplen - length;
 		caplen -= fcslen;
 		ndo->ndo_snapend -= fcslen;
 	}
 
 	if (caplen < IEEE802_11_FC_LEN) {
 		ND_PRINT((ndo, "%s", tstr));
 		return orig_caplen;
 	}
 
 	fc = EXTRACT_LE_16BITS(p);
 	hdrlen = extract_header_length(ndo, fc);
 	if (hdrlen == 0) {
 		 
 		return (0);
 	}
 	if (pad)
  		hdrlen = roundup2(hdrlen, 4);
  	if (ndo->ndo_Hflag && FC_TYPE(fc) == T_DATA &&
  	    DATA_FRAME_IS_QOS(FC_SUBTYPE(fc))) {
// 		if (caplen < hdrlen + 1) {
// 			ND_PRINT((ndo, "%s", tstr));
// 			return hdrlen;
// 		}
  		meshdrlen = extract_mesh_header_length(p+hdrlen);
  		hdrlen += meshdrlen;
  	} else
 		meshdrlen = 0;
 
 	if (caplen < hdrlen) {
 		ND_PRINT((ndo, "%s", tstr));
 		return hdrlen;
 	}
 
 	if (ndo->ndo_eflag)
 		ieee_802_11_hdr_print(ndo, fc, p, hdrlen, meshdrlen);
 
 	 
 	length -= hdrlen;
 	caplen -= hdrlen;
 	p += hdrlen;
 
 	src.addr_string = etheraddr_string;
 	dst.addr_string = etheraddr_string;
 	switch (FC_TYPE(fc)) {
 	case T_MGMT:
 		get_mgmt_src_dst_mac(p - hdrlen, &src.addr, &dst.addr);
 		if (!mgmt_body_print(ndo, fc, src.addr, p, length)) {
 			ND_PRINT((ndo, "%s", tstr));
 			return hdrlen;
 		}
 		break;
 	case T_CTRL:
 		if (!ctrl_body_print(ndo, fc, p - hdrlen)) {
 			ND_PRINT((ndo, "%s", tstr));
 			return hdrlen;
 		}
 		break;
 	case T_DATA:
 		if (DATA_FRAME_IS_NULL(FC_SUBTYPE(fc)))
 			return hdrlen;	 
 		 
 		if (FC_PROTECTED(fc)) {
 			ND_PRINT((ndo, "Data"));
 			if (!wep_print(ndo, p)) {
 				ND_PRINT((ndo, "%s", tstr));
 				return hdrlen;
 			}
 		} else {
 			get_data_src_dst_mac(fc, p - hdrlen, &src.addr, &dst.addr);
 			llc_hdrlen = llc_print(ndo, p, length, caplen, &src, &dst);
 			if (llc_hdrlen < 0) {
 				 
 				if (!ndo->ndo_suppress_default_print)
 					ND_DEFAULTPRINT(p, caplen);
 				llc_hdrlen = -llc_hdrlen;
 			}
 			hdrlen += llc_hdrlen;
 		}
 		break;
 	default:
 		 
 		break;
 	}
 
 	return hdrlen;
 }