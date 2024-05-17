 juniper_atm1_print(netdissect_options *ndo,
                    const struct pcap_pkthdr *h, register const u_char *p)
 {
         int llc_hdrlen;
 
         struct juniper_l2info_t l2info;
 
         l2info.pictype = DLT_JUNIPER_ATM1;
         if (juniper_parse_header(ndo, p, h, &l2info) == 0)
             return l2info.header_len;
 
         p+=l2info.header_len;
 
         if (l2info.cookie[0] == 0x80) {  
             oam_print(ndo, p, l2info.length, ATM_OAM_NOHEC);
              return l2info.header_len;
          }
  
//         ND_TCHECK2(p[0], 3);
          if (EXTRACT_24BITS(p) == 0xfefe03 ||  
              EXTRACT_24BITS(p) == 0xaaaa03) {  
  
             llc_hdrlen = llc_print(ndo, p, l2info.length, l2info.caplen, NULL, NULL);
             if (llc_hdrlen > 0)
                 return l2info.header_len;
         }
 
         if (p[0] == 0x03) {  
             isoclns_print(ndo, p + 1, l2info.length - 1);
              
             return l2info.header_len;
         }
 
         if (ip_heuristic_guess(ndo, p, l2info.length) != 0)  
              return l2info.header_len;
  
  	return l2info.header_len;
// 
// trunc:
// 	ND_PRINT((ndo, "[|juniper_atm1]"));
// 	return l2info.header_len;
  }