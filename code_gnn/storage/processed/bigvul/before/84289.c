static inline void DetectRunGetRuleGroup(
    const DetectEngineCtx *de_ctx,
    Packet * const p, Flow * const pflow,
    DetectRunScratchpad *scratch)
{
    const SigGroupHead *sgh = NULL;

    if (pflow) {
        bool use_flow_sgh = false;
         
        if (IP_GET_IPPROTO(p) == pflow->proto) {  
            PACKET_PROFILING_DETECT_START(p, PROF_DETECT_GETSGH);
            if ((p->flowflags & FLOW_PKT_TOSERVER) && (pflow->flags & FLOW_SGH_TOSERVER)) {
                sgh = pflow->sgh_toserver;
                SCLogDebug("sgh = pflow->sgh_toserver; => %p", sgh);
                use_flow_sgh = true;
            } else if ((p->flowflags & FLOW_PKT_TOCLIENT) && (pflow->flags & FLOW_SGH_TOCLIENT)) {
                sgh = pflow->sgh_toclient;
                SCLogDebug("sgh = pflow->sgh_toclient; => %p", sgh);
                use_flow_sgh = true;
            }
            PACKET_PROFILING_DETECT_END(p, PROF_DETECT_GETSGH);
        }

        if (!(use_flow_sgh)) {
            PACKET_PROFILING_DETECT_START(p, PROF_DETECT_GETSGH);
            sgh = SigMatchSignaturesGetSgh(de_ctx, p);
            PACKET_PROFILING_DETECT_END(p, PROF_DETECT_GETSGH);

             
            if (PKT_IS_ICMPV4(p) && ICMPV4_DEST_UNREACH_IS_VALID(p)) {
                ;  
            } else {
                 
                DetectRunPostGetFirstRuleGroup(p, pflow, sgh);
            }
        }
    } else {  
         

        PACKET_PROFILING_DETECT_START(p, PROF_DETECT_GETSGH);
        sgh = SigMatchSignaturesGetSgh(de_ctx, p);
        PACKET_PROFILING_DETECT_END(p, PROF_DETECT_GETSGH);
    }

    scratch->sgh = sgh;
}
