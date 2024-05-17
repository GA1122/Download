static inline void DetectRunPostRules(
    ThreadVars *tv,
    DetectEngineCtx *de_ctx,
    DetectEngineThreadCtx *det_ctx,
    Packet * const p,
    Flow * const pflow,
    DetectRunScratchpad *scratch)
{
     
    if (pflow && pflow->alstate && AppLayerParserProtocolSupportsTxs(p->proto, scratch->alproto)) {
        PACKET_PROFILING_DETECT_START(p, PROF_DETECT_TX_UPDATE);
        DeStateUpdateInspectTransactionId(pflow, scratch->flow_flags, (scratch->sgh == NULL));
        PACKET_PROFILING_DETECT_END(p, PROF_DETECT_TX_UPDATE);
    }

     
     

    PACKET_PROFILING_DETECT_START(p, PROF_DETECT_ALERT);
    PacketAlertFinalize(de_ctx, det_ctx, p);
    if (p->alerts.cnt > 0) {
        StatsAddUI64(tv, det_ctx->counter_alerts, (uint64_t)p->alerts.cnt);
    }
    PACKET_PROFILING_DETECT_END(p, PROF_DETECT_ALERT);
}
