 DecodeIPV6ExtHdrs(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, uint8_t *pkt, uint16_t len, PacketQueue *pq)
 {
      SCEnter();
  
      uint8_t *orig_pkt = pkt;
    uint8_t nh = 0;  
//     uint8_t nh = IPV6_GET_NH(p);  
      uint16_t hdrextlen = 0;
    uint16_t plen;
//     uint16_t plen = len;
      char dstopts = 0;
      char exthdr_fh_done = 0;
      int hh = 0;
      int rh = 0;
      int eh = 0;
      int ah = 0;
  
    nh = IPV6_GET_NH(p);
    plen = len;
      while(1)
      {
         
        if (nh == IPPROTO_NONE && plen > 0) {
            ENGINE_SET_EVENT(p, IPV6_DATA_AFTER_NONE_HEADER);
//         if (nh == IPPROTO_NONE) {
//             if (plen > 0) {
//                  
//                 ENGINE_SET_EVENT(p, IPV6_DATA_AFTER_NONE_HEADER);
//             }
              SCReturn;
          }
  
          if (plen < 2) {  
//             ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
              SCReturn;
          }
  
         switch(nh)
         {
             case IPPROTO_TCP:
                 IPV6_SET_L4PROTO(p,nh);
                 DecodeTCP(tv, dtv, p, pkt, plen, pq);
                 SCReturn;
 
             case IPPROTO_UDP:
                 IPV6_SET_L4PROTO(p,nh);
                 DecodeUDP(tv, dtv, p, pkt, plen, pq);
                 SCReturn;
 
             case IPPROTO_ICMPV6:
                 IPV6_SET_L4PROTO(p,nh);
                 DecodeICMPV6(tv, dtv, p, pkt, plen, pq);
                 SCReturn;
 
             case IPPROTO_SCTP:
                 IPV6_SET_L4PROTO(p,nh);
                 DecodeSCTP(tv, dtv, p, pkt, plen, pq);
                 SCReturn;
 
             case IPPROTO_ROUTING:
                 IPV6_SET_L4PROTO(p,nh);
                 hdrextlen = 8 + (*(pkt+1) * 8);   
 
                  SCLogDebug("hdrextlen %"PRIu8, hdrextlen);
  
                  if (hdrextlen > plen) {
                    ENGINE_SET_EVENT(p, IPV6_TRUNC_EXTHDR);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
                      SCReturn;
                  }
  
                 if (rh) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_DUPL_RH);
                      
                     nh = *pkt;
                     pkt += hdrextlen;
                     plen -= hdrextlen;
                     break;
                 }
 
                 rh = 1;
                 IPV6_EXTHDR_SET_RH(p);
 
                 uint8_t ip6rh_type = *(pkt + 2);
                 if (ip6rh_type == 0) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_RH_TYPE_0);
                 }
                 p->ip6eh.rh_type = ip6rh_type;
 
                 nh = *pkt;
                 pkt += hdrextlen;
                 plen -= hdrextlen;
                 break;
 
             case IPPROTO_HOPOPTS:
             case IPPROTO_DSTOPTS:
             {
                 IPV6OptHAO hao_s, *hao = &hao_s;
                 IPV6OptRA ra_s, *ra = &ra_s;
                 IPV6OptJumbo jumbo_s, *jumbo = &jumbo_s;
                 uint16_t optslen = 0;
 
                  IPV6_SET_L4PROTO(p,nh);
                  hdrextlen =  (*(pkt+1) + 1) << 3;
                  if (hdrextlen > plen) {
                    ENGINE_SET_EVENT(p, IPV6_TRUNC_EXTHDR);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
                      SCReturn;
                  }
  
                 uint8_t *ptr = pkt + 2;  
 
                  
                 if (nh == IPPROTO_HOPOPTS) {
                     if (hh) {
                         ENGINE_SET_EVENT(p, IPV6_EXTHDR_DUPL_HH);
                          
                         nh = *pkt;
                         pkt += hdrextlen;
                         plen -= hdrextlen;
                         break;
                     }
 
                     hh = 1;
 
                     optslen = ((*(pkt + 1) + 1 ) << 3) - 2;
                 }
                 else if (nh == IPPROTO_DSTOPTS)
                 {
                     if (dstopts == 0) {
                         optslen = ((*(pkt + 1) + 1 ) << 3) - 2;
                         dstopts = 1;
                     } else if (dstopts == 1) {
                         optslen = ((*(pkt + 1) + 1 ) << 3) - 2;
                         dstopts = 2;
                     } else {
                         ENGINE_SET_EVENT(p, IPV6_EXTHDR_DUPL_DH);
                          
                         nh = *pkt;
                         pkt += hdrextlen;
                         plen -= hdrextlen;
                         break;
                     }
                 }
 
                  if (optslen > plen) {
                       
                    ENGINE_SET_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
                       
                      nh = *pkt;
                     pkt += hdrextlen;
                     plen -= hdrextlen;
                     break;
                 }
  
                 uint16_t padn_cnt = 0;
                 uint16_t other_cnt = 0;
                 uint16_t offset = 0;
                 while(offset < optslen)
                 {
                     if (*ptr == IPV6OPT_PAD1)
                     {
                         padn_cnt++;
                         offset++;
                         ptr++;
                         continue;
                      }
  
                      if (offset + 1 >= optslen) {
                        ENGINE_SET_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
//                         ENGINE_SET_INVALID_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
                          break;
                      }
  
                      
                     uint8_t ip6_optlen = *(ptr + 1);
  
                       
                      if ((offset + 1 + ip6_optlen) > optslen) {
                        ENGINE_SET_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
//                         ENGINE_SET_INVALID_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
                          break;
                      }
  
                     if (*ptr == IPV6OPT_PADN)  
                     {
                         padn_cnt++;
 
                          
                         if (ip6_optlen == 0)
                             ENGINE_SET_EVENT(p, IPV6_EXTHDR_ZERO_LEN_PADN);
                     }
                     else if (*ptr == IPV6OPT_RA)  
                     {
                         ra->ip6ra_type = *(ptr);
                          ra->ip6ra_len  = ip6_optlen;
  
                          if (ip6_optlen < sizeof(ra->ip6ra_value)) {
                            ENGINE_SET_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
//                             ENGINE_SET_INVALID_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
                              break;
                          }
  
                         memcpy(&ra->ip6ra_value, (ptr + 2), sizeof(ra->ip6ra_value));
                         ra->ip6ra_value = SCNtohs(ra->ip6ra_value);
                         other_cnt++;
                     }
                     else if (*ptr == IPV6OPT_JUMBO)  
                     {
                         jumbo->ip6j_type = *(ptr);
                          jumbo->ip6j_len  = ip6_optlen;
  
                          if (ip6_optlen < sizeof(jumbo->ip6j_payload_len)) {
                            ENGINE_SET_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
//                             ENGINE_SET_INVALID_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
                              break;
                          }
  
                         memcpy(&jumbo->ip6j_payload_len, (ptr+2), sizeof(jumbo->ip6j_payload_len));
                         jumbo->ip6j_payload_len = SCNtohl(jumbo->ip6j_payload_len);
                     }
                     else if (*ptr == IPV6OPT_HAO)  
                     {
                         hao->ip6hao_type = *(ptr);
                          hao->ip6hao_len  = ip6_optlen;
  
                          if (ip6_optlen < sizeof(hao->ip6hao_hoa)) {
                            ENGINE_SET_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
//                             ENGINE_SET_INVALID_EVENT(p, IPV6_EXTHDR_INVALID_OPTLEN);
                              break;
                          }
  
                         memcpy(&hao->ip6hao_hoa, (ptr+2), sizeof(hao->ip6hao_hoa));
                         other_cnt++;
                     } else {
                         if (nh == IPPROTO_HOPOPTS)
                             ENGINE_SET_EVENT(p, IPV6_HOPOPTS_UNKNOWN_OPT);
                         else
                             ENGINE_SET_EVENT(p, IPV6_DSTOPTS_UNKNOWN_OPT);
 
                         other_cnt++;
                     }
                     uint16_t optlen = (*(ptr + 1) + 2);
                     ptr += optlen;  
                     offset += optlen;
                 }
                  
                 if (padn_cnt > 0 && other_cnt == 0) {
                     if (nh == IPPROTO_HOPOPTS)
                         ENGINE_SET_EVENT(p, IPV6_HOPOPTS_ONLY_PADDING);
                     else
                         ENGINE_SET_EVENT(p, IPV6_DSTOPTS_ONLY_PADDING);
                 }
 
                 nh = *pkt;
                 pkt += hdrextlen;
                 plen -= hdrextlen;
                 break;
             }
 
             case IPPROTO_FRAGMENT:
             {
                 IPV6_SET_L4PROTO(p,nh);
                  
                 if (exthdr_fh_done == 0) {
                     p->ip6eh.fh_offset = pkt - orig_pkt;
                     exthdr_fh_done = 1;
                 }
 
                  uint16_t prev_hdrextlen = hdrextlen;
                  hdrextlen = sizeof(IPV6FragHdr);
                  if (hdrextlen > plen) {
                    ENGINE_SET_EVENT(p, IPV6_TRUNC_EXTHDR);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
                      SCReturn;
                  }
  
                  
                 if (*(pkt + 1) != 0) {
                     ENGINE_SET_EVENT(p, IPV6_FH_NON_ZERO_RES_FIELD);
                      
                 }
 
                 if (IPV6_EXTHDR_ISSET_FH(p)) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_DUPL_FH);
                     nh = *pkt;
                     pkt += hdrextlen;
                     plen -= hdrextlen;
                     break;
                 }
 
                  
                 IPV6_EXTHDR_SET_FH(p);
 
                  
                 DecodeIPV6FragHeader(p, pkt, hdrextlen, plen, prev_hdrextlen);
 
                  
                 if (p->ip6eh.fh_more_frags_set == 0 && p->ip6eh.fh_offset == 0) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_USELESS_FH);
 
                     nh = *pkt;
                     pkt += hdrextlen;
                     plen -= hdrextlen;
                     break;
                 }
 
                  
                 p->flags |= PKT_IS_FRAGMENT;
                 SCReturn;
             }
             case IPPROTO_ESP:
             {
                  IPV6_SET_L4PROTO(p,nh);
                  hdrextlen = sizeof(IPV6EspHdr);
                  if (hdrextlen > plen) {
                    ENGINE_SET_EVENT(p, IPV6_TRUNC_EXTHDR);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
                      SCReturn;
                  }
  
                 if (eh) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_DUPL_EH);
                     SCReturn;
                 }
 
                 eh = 1;
 
                 nh = IPPROTO_NONE;
                 pkt += hdrextlen;
                 plen -= hdrextlen;
                 break;
             }
             case IPPROTO_AH:
             {
                 IPV6_SET_L4PROTO(p,nh);
                  
                 hdrextlen = sizeof(IPV6AuthHdr);
                  
                 if (*(pkt+1) > 0)
                     hdrextlen += ((*(pkt+1) - 1) * 4);
 
                  SCLogDebug("hdrextlen %"PRIu8, hdrextlen);
  
                  if (hdrextlen > plen) {
                    ENGINE_SET_EVENT(p, IPV6_TRUNC_EXTHDR);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
                      SCReturn;
                  }
  
                 IPV6AuthHdr *ahhdr = (IPV6AuthHdr *)pkt;
                 if (ahhdr->ip6ah_reserved != 0x0000) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_AH_RES_NOT_NULL);
                 }
 
                 if (ah) {
                     ENGINE_SET_EVENT(p, IPV6_EXTHDR_DUPL_AH);
                     nh = *pkt;
                     pkt += hdrextlen;
                     plen -= hdrextlen;
                     break;
                 }
 
                 ah = 1;
 
                 nh = *pkt;
                 pkt += hdrextlen;
                 plen -= hdrextlen;
                 break;
             }
             case IPPROTO_IPIP:
                 IPV6_SET_L4PROTO(p,nh);
                 DecodeIPv4inIPv6(tv, dtv, p, pkt, plen, pq);
                 SCReturn;
              
             case IPPROTO_NONE:
                 IPV6_SET_L4PROTO(p,nh);
                 SCReturn;
             case IPPROTO_ICMP:
                 ENGINE_SET_EVENT(p,IPV6_WITH_ICMPV4);
                 SCReturn;
              
             case IPPROTO_MH:
             case IPPROTO_HIP:
              case IPPROTO_SHIM6:
                  hdrextlen = 8 + (*(pkt+1) * 8);   
                  if (hdrextlen > plen) {
                    ENGINE_SET_EVENT(p, IPV6_TRUNC_EXTHDR);
//                     ENGINE_SET_INVALID_EVENT(p, IPV6_TRUNC_EXTHDR);
                      SCReturn;
                  }
                  nh = *pkt;
                 pkt += hdrextlen;
                 plen -= hdrextlen;
                 break;
             default:
                 ENGINE_SET_EVENT(p, IPV6_UNKNOWN_NEXT_HEADER);
                 IPV6_SET_L4PROTO(p,nh);
                 SCReturn;
         }
     }
 
     SCReturn;
 }