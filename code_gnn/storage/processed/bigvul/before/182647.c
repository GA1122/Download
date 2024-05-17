 static void DetectRunCleanup(DetectEngineThreadCtx *det_ctx,
         Packet *p, Flow * const pflow)
 {
     PACKET_PROFILING_DETECT_START(p, PROF_DETECT_CLEANUP);
      
     PacketPatternCleanup(det_ctx);
  
      if (pflow != NULL) {
           
        if (p->proto == IPPROTO_TCP && pflow->protoctx != NULL) {
//         if (p->proto == IPPROTO_TCP && pflow->protoctx != NULL &&
//             (p->flags & PKT_STREAM_EST))
//         {
              StreamReassembleRawUpdateProgress(pflow->protoctx, p,
                      det_ctx->raw_stream_progress);
  
             DetectEngineCleanHCBDBuffers(det_ctx);
         }
     }
     PACKET_PROFILING_DETECT_END(p, PROF_DETECT_CLEANUP);
     SCReturn;
 }