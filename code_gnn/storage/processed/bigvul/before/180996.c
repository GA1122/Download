 rpki_rtr_pdu_print (netdissect_options *ndo, const u_char *tptr, u_int indent)
// static u_int
// rpki_rtr_pdu_print (netdissect_options *ndo, const u_char *tptr, const u_int len,
// 	const u_char recurse, const u_int indent)
  {
      const rpki_rtr_pdu *pdu_header;
      u_int pdu_type, pdu_len, hexdump;
      const u_char *msg;
  
//      
//     ND_TCHECK_8BITS(tptr);
//     if (*tptr != 0) {
// 	 
// 	ND_PRINT((ndo, "%sRPKI-RTRv%u (unknown)", indent_string(8), *tptr));
// 	return len;
//     }
//     if (len < sizeof(rpki_rtr_pdu)) {
// 	ND_PRINT((ndo, "(%u bytes is too few to decode)", len));
// 	goto invalid;
//     }
//     ND_TCHECK2(*tptr, sizeof(rpki_rtr_pdu));
      pdu_header = (const rpki_rtr_pdu *)tptr;
      pdu_type = pdu_header->pdu_type;
      pdu_len = EXTRACT_32BITS(pdu_header->length);
    ND_TCHECK2(*tptr, pdu_len);
//      
      hexdump = FALSE;
  
      ND_PRINT((ndo, "%sRPKI-RTRv%u, %s PDU (%u), length: %u",
  	   indent_string(8),
  	   pdu_header->version,
  	   tok2str(rpki_rtr_pdu_values, "Unknown", pdu_type),
  	   pdu_type, pdu_len));
//     if (pdu_len < sizeof(rpki_rtr_pdu) || pdu_len > len)
// 	goto invalid;
  
      switch (pdu_type) {
  
 	 
      case RPKI_RTR_SERIAL_NOTIFY_PDU:
      case RPKI_RTR_SERIAL_QUERY_PDU:
      case RPKI_RTR_END_OF_DATA_PDU:
// 	if (pdu_len != sizeof(rpki_rtr_pdu) + 4)
// 	    goto invalid;
// 	ND_TCHECK2(*tptr, pdu_len);
          msg = (const u_char *)(pdu_header + 1);
  	ND_PRINT((ndo, "%sSession ID: 0x%04x, Serial: %u",
  	       indent_string(indent+2),
 	       EXTRACT_16BITS(pdu_header->u.session_id),
 	       EXTRACT_32BITS(msg)));
 	break;
 
 	 
      case RPKI_RTR_RESET_QUERY_PDU:
      case RPKI_RTR_CACHE_RESET_PDU:
// 	if (pdu_len != sizeof(rpki_rtr_pdu))
// 	    goto invalid;
// 	 
  
  	 
  	break;
  
      case RPKI_RTR_CACHE_RESPONSE_PDU:
// 	if (pdu_len != sizeof(rpki_rtr_pdu))
// 	    goto invalid;
// 	 
  	ND_PRINT((ndo, "%sSession ID: 0x%04x",
  	       indent_string(indent+2),
  	       EXTRACT_16BITS(pdu_header->u.session_id)));
 	break;
 
     case RPKI_RTR_IPV4_PREFIX_PDU:
  	{
  	    const rpki_rtr_pdu_ipv4_prefix *pdu;
  
// 	    if (pdu_len != sizeof(rpki_rtr_pdu) + 12)
// 		goto invalid;
// 	    ND_TCHECK2(*tptr, pdu_len);
  	    pdu = (const rpki_rtr_pdu_ipv4_prefix *)tptr;
  	    ND_PRINT((ndo, "%sIPv4 Prefix %s/%u-%u, origin-as %u, flags 0x%02x",
  		   indent_string(indent+2),
 		   ipaddr_string(ndo, pdu->prefix),
 		   pdu->prefix_length, pdu->max_length,
 		   EXTRACT_32BITS(pdu->as), pdu->flags));
 	}
 	break;
 
     case RPKI_RTR_IPV6_PREFIX_PDU:
  	{
  	    const rpki_rtr_pdu_ipv6_prefix *pdu;
  
// 	    if (pdu_len != sizeof(rpki_rtr_pdu) + 24)
// 		goto invalid;
// 	    ND_TCHECK2(*tptr, pdu_len);
  	    pdu = (const rpki_rtr_pdu_ipv6_prefix *)tptr;
  	    ND_PRINT((ndo, "%sIPv6 Prefix %s/%u-%u, origin-as %u, flags 0x%02x",
  		   indent_string(indent+2),
 		   ip6addr_string(ndo, pdu->prefix),
 		   pdu->prefix_length, pdu->max_length,
 		   EXTRACT_32BITS(pdu->as), pdu->flags));
 	}
 	break;
 
     case RPKI_RTR_ERROR_REPORT_PDU:
 	{
  	    const rpki_rtr_pdu_error_report *pdu;
  	    u_int encapsulated_pdu_length, text_length, tlen, error_code;
  
// 	    tlen = sizeof(rpki_rtr_pdu);
// 	     
// 	    if (pdu_len < tlen + 4)
// 		goto invalid;
// 	    ND_TCHECK2(*tptr, tlen + 4);
// 	     
  	    pdu = (const rpki_rtr_pdu_error_report *)tptr;
  	    encapsulated_pdu_length = EXTRACT_32BITS(pdu->encapsulated_pdu_length);
	    ND_TCHECK2(*tptr, encapsulated_pdu_length);
	    tlen = pdu_len;
// 	    tlen += 4;
  
  	    error_code = EXTRACT_16BITS(pdu->pdu_header.u.error_code);
  	    ND_PRINT((ndo, "%sError code: %s (%u), Encapsulated PDU length: %u",
  		   indent_string(indent+2),
  		   tok2str(rpki_rtr_error_codes, "Unknown", error_code),
  		   error_code, encapsulated_pdu_length));
  
	    tptr += sizeof(*pdu);
	    tlen -= sizeof(*pdu);
	     
	    if (encapsulated_pdu_length &&
		(encapsulated_pdu_length <= tlen)) {
		ND_PRINT((ndo, "%s-----encapsulated PDU-----", indent_string(indent+4)));
		if (rpki_rtr_pdu_print(ndo, tptr, indent+2))
			goto trunc;
// 	    if (encapsulated_pdu_length) {
// 		 
// 		if (pdu_len < tlen + encapsulated_pdu_length)
// 		    goto invalid;
// 		if (! recurse) {
// 		    ND_TCHECK2(*tptr, tlen + encapsulated_pdu_length);
// 		}
// 		else {
// 		    ND_PRINT((ndo, "%s-----encapsulated PDU-----", indent_string(indent+4)));
// 		    rpki_rtr_pdu_print(ndo, tptr + tlen,
// 			encapsulated_pdu_length, 0, indent + 2);
// 		}
// 		tlen += encapsulated_pdu_length;
  	    }
  
	    tptr += encapsulated_pdu_length;
	    tlen -= encapsulated_pdu_length;
// 	    if (pdu_len < tlen + 4)
// 		goto invalid;
// 	    ND_TCHECK2(*tptr, tlen + 4);
// 	     
  
  	     
	    text_length = 0;
	    if (tlen > 4) {
		text_length = EXTRACT_32BITS(tptr);
		tptr += 4;
		tlen -= 4;
	    }
	    ND_TCHECK2(*tptr, text_length);
	    if (text_length && (text_length <= tlen )) {
// 	    text_length = EXTRACT_32BITS(tptr + tlen);
// 	    tlen += 4;
// 
// 	    if (text_length) {
// 		if (pdu_len < tlen + text_length)
// 		    goto invalid;
// 		 
  		ND_PRINT((ndo, "%sError text: ", indent_string(indent+2)));
		if (fn_printn(ndo, tptr, text_length, ndo->ndo_snapend))
// 		if (fn_printn(ndo, tptr + tlen, text_length, ndo->ndo_snapend))
  			goto trunc;
  	    }
  	}
  	break;
  
      default:
// 	ND_TCHECK2(*tptr, pdu_len);
  
  	 
 	hexdump = TRUE;
     }
 
      
      if (ndo->ndo_vflag > 1 || (ndo->ndo_vflag && hexdump)) {
  	print_unknown_data(ndo,tptr,"\n\t  ", pdu_len);
      }
    return 0;
//     return pdu_len;
  
// invalid:
//     ND_PRINT((ndo, "%s", istr));
//     ND_TCHECK2(*tptr, len);
//     return len;
  trunc:
    return 1;
//     ND_PRINT((ndo, "\n\t%s", tstr));
//     return len;
  }