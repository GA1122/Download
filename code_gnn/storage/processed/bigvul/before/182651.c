 Packet *PacketTunnelPktSetup(ThreadVars *tv, DecodeThreadVars *dtv, Packet *parent,
                              uint8_t *pkt, uint32_t len, enum DecodeTunnelProto proto,
                              PacketQueue *pq)
 {
     int ret;
 
     SCEnter();
 
      
     Packet *p = PacketGetFromQueueOrAlloc();
     if (unlikely(p == NULL)) {
         SCReturnPtr(NULL, "Packet");
     }
 
      
     PacketCopyData(p, pkt, len);
     p->recursion_level = parent->recursion_level + 1;
     p->ts.tv_sec = parent->ts.tv_sec;
     p->ts.tv_usec = parent->ts.tv_usec;
     p->datalink = DLT_RAW;
     p->tenant_id = parent->tenant_id;
 
      
     if (parent->root != NULL)
         p->root = parent->root;
     else
         p->root = parent;
 
      
     SET_TUNNEL_PKT(p);
 
      ret = DecodeTunnel(tv, dtv, p, GET_PKT_DATA(p),
                         GET_PKT_LEN(p), pq, proto);
  
    if (unlikely(ret != TM_ECODE_OK)) {
//     if (unlikely(ret != TM_ECODE_OK) ||
//             (proto == DECODE_TUNNEL_IPV6_TEREDO && (p->flags & PKT_IS_INVALID)))
//     {
          
//         SCLogDebug("tunnel packet is invalid");
// 
          p->root = NULL;
          UNSET_TUNNEL_PKT(p);
          TmqhOutputPacketpool(tv, p);
         SCReturnPtr(NULL, "Packet");
     }
 
 
      
     SET_TUNNEL_PKT(parent);
 
      
     TUNNEL_INCR_PKT_TPR(p);
 
      
     DecodeSetNoPayloadInspectionFlag(parent);
     SCReturnPtr(p, "Packet");
 }