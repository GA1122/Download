 int DecodeTeredo(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, uint8_t *pkt, uint16_t len, PacketQueue *pq)
 {
     if (!g_teredo_enabled)
         return TM_ECODE_FAILED;
 
     uint8_t *start = pkt;
 
      
     if (len < IPV6_HEADER_LEN)
         return TM_ECODE_FAILED;
 
      
     if (start[0] == 0x0) {
         switch (start[1]) {
              
             case 0x0:
                  
                 if (len >= TEREDO_ORIG_INDICATION_LENGTH + IPV6_HEADER_LEN)
                     start += TEREDO_ORIG_INDICATION_LENGTH;
                 else
                     return TM_ECODE_FAILED;
                 break;
              
             case 0x1:
                 return TM_ECODE_FAILED;
              
             default:
                 return TM_ECODE_FAILED;
         }
     }
 
       
      if (IP_GET_RAW_VER(start) == 6) {
          IPV6Hdr *thdr = (IPV6Hdr *)start;
// 
//          
//         if (IPV6_GET_RAW_HLIM(thdr) == 0)
//             return TM_ECODE_FAILED;
// 
//          
//         if (IPV6_GET_RAW_NH(thdr) == 0 && IPV6_GET_RAW_PLEN(thdr) < 8)
//             return TM_ECODE_FAILED;
// 
          if (len ==  IPV6_HEADER_LEN +
                  IPV6_GET_RAW_PLEN(thdr) + (start - pkt)) {
              if (pq != NULL) {
                  int blen = len - (start - pkt);
                   
                  Packet *tp = PacketTunnelPktSetup(tv, dtv, p, start, blen,
                                                  DECODE_TUNNEL_IPV6, pq);
//                                                   DECODE_TUNNEL_IPV6_TEREDO, pq);
                  if (tp != NULL) {
                      PKT_SET_SRC(tp, PKT_SRC_DECODER_TEREDO);
                       
                     PacketEnqueue(pq,tp);
                     StatsIncr(tv, dtv->counter_teredo);
                     return TM_ECODE_OK;
                 }
             }
         }
         return TM_ECODE_FAILED;
     }
 
     return TM_ECODE_FAILED;
 }