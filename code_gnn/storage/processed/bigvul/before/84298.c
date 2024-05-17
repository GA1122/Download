static bool DetectRunTxInspectRule(ThreadVars *tv,
        DetectEngineCtx *de_ctx,
        DetectEngineThreadCtx *det_ctx,
        Packet *p,
        Flow *f,
        const uint8_t flow_flags,    
        void *alstate,
        DetectTransaction *tx,
        const Signature *s,
        uint32_t *stored_flags,
        RuleMatchCandidateTx *can,
        DetectRunScratchpad *scratch)
{
    const int direction = (flow_flags & STREAM_TOSERVER) ? 0 : 1;
    uint32_t inspect_flags = stored_flags ? *stored_flags : 0;
    int total_matches = 0;
    int file_no_match = 0;
    bool retval = false;
    bool mpm_before_progress = false;    
    bool mpm_in_progress = false;        

    TRACE_SID_TXS(s->id, tx, "starting %s", direction ? "toclient" : "toserver");

     
    if (likely(stored_flags == NULL)) {
        TRACE_SID_TXS(s->id, tx, "first inspect, run packet matches");
        if (DetectRunInspectRuleHeader(p, f, s, s->flags, s->proto.flags) == 0) {
            TRACE_SID_TXS(s->id, tx, "DetectRunInspectRuleHeader() no match");
            return false;
        }
        if (DetectRunInspectRulePacketMatches(tv, det_ctx, p, f, s) == 0) {
            TRACE_SID_TXS(s->id, tx, "DetectRunInspectRulePacketMatches no match");
            return false;
        }
         
        if (!(f->alproto == s->alproto || s->alproto == ALPROTO_UNKNOWN)) {
            TRACE_SID_TXS(s->id, tx, "alproto mismatch");
            return false;
        }
    }

    const DetectEngineAppInspectionEngine *engine = s->app_inspect;
    while (engine != NULL) {  
        TRACE_SID_TXS(s->id, tx, "engine %p inspect_flags %x", engine, inspect_flags);
        if (!(inspect_flags & BIT_U32(engine->id)) &&
                direction == engine->dir)
        {
             
            if (tx->tx_progress < engine->progress) {
                SCLogDebug("tx progress %d < engine progress %d",
                        tx->tx_progress, engine->progress);
                break;
            }
            if (engine->mpm) {
                if (tx->tx_progress > engine->progress) {
                    mpm_before_progress = true;
                } else if (tx->tx_progress == engine->progress) {
                    mpm_in_progress = true;
                }
            }

             
            int match;
            if (unlikely(engine->stream && can->stream_stored)) {
                match = can->stream_result;
                TRACE_SID_TXS(s->id, tx, "stream skipped, stored result %d used instead", match);
             
            } else if (unlikely(engine->alproto != 0 && engine->alproto != f->alproto)) {
                inspect_flags |= BIT_U32(engine->id);
                engine = engine->next;
                total_matches++;
                continue;
            } else {
                KEYWORD_PROFILING_SET_LIST(det_ctx, engine->sm_list);
                match = engine->Callback(tv, de_ctx, det_ctx,
                        s, engine->smd, f, flow_flags, alstate, tx->tx_ptr, tx->tx_id);
                TRACE_SID_TXS(s->id, tx, "engine %p match %d", engine, match);
                if (engine->stream) {
                    can->stream_stored = true;
                    can->stream_result = match;
                    TRACE_SID_TXS(s->id, tx, "stream ran, store result %d for next tx (if any)", match);
                }
            }
            if (match == DETECT_ENGINE_INSPECT_SIG_MATCH) {
                inspect_flags |= BIT_U32(engine->id);
                engine = engine->next;
                total_matches++;
                continue;
            } else if (match == DETECT_ENGINE_INSPECT_SIG_MATCH_MORE_FILES) {
                 
                engine = engine->next;
                total_matches++;
                continue;
            } else if (match == DETECT_ENGINE_INSPECT_SIG_CANT_MATCH) {
                inspect_flags |= DE_STATE_FLAG_SIG_CANT_MATCH;
                inspect_flags |= BIT_U32(engine->id);
            } else if (match == DETECT_ENGINE_INSPECT_SIG_CANT_MATCH_FILESTORE) {
                inspect_flags |= DE_STATE_FLAG_SIG_CANT_MATCH;
                inspect_flags |= BIT_U32(engine->id);
                file_no_match = 1;
            }
             
            if (engine->mpm && mpm_before_progress) {
                inspect_flags |= DE_STATE_FLAG_SIG_CANT_MATCH;
                inspect_flags |= BIT_U32(engine->id);
            }
            break;
        }
        engine = engine->next;
    }
    TRACE_SID_TXS(s->id, tx, "inspect_flags %x, total_matches %u, engine %p",
            inspect_flags, total_matches, engine);

    if (engine == NULL && total_matches) {
        inspect_flags |= DE_STATE_FLAG_FULL_INSPECT;
        TRACE_SID_TXS(s->id, tx, "MATCH");
        retval = true;
    }

    if (stored_flags) {
        *stored_flags = inspect_flags;
        TRACE_SID_TXS(s->id, tx, "continue inspect flags %08x", inspect_flags);
    } else {

        TRACE_SID_TXS(s->id, tx, "start inspect flags %08x", inspect_flags);
        if (inspect_flags & DE_STATE_FLAG_SIG_CANT_MATCH) {
            if (file_no_match) {
                 
                DetectRunStoreStateTx(scratch->sgh, f, tx->tx_ptr, tx->tx_id, s,
                        inspect_flags, flow_flags, file_no_match);
            }
        } else if ((inspect_flags & DE_STATE_FLAG_FULL_INSPECT) && mpm_before_progress) {
            TRACE_SID_TXS(s->id, tx, "no need to store match sig, "
                    "mpm won't trigger for it anymore");

            if (inspect_flags & DE_STATE_FLAG_FILE_INSPECT) {
                TRACE_SID_TXS(s->id, tx, "except that for new files, "
                        "we may have to revisit anyway");
                DetectRunStoreStateTx(scratch->sgh, f, tx->tx_ptr, tx->tx_id, s,
                        inspect_flags, flow_flags, file_no_match);
            }
        } else if ((inspect_flags & DE_STATE_FLAG_FULL_INSPECT) == 0 && mpm_in_progress) {
            TRACE_SID_TXS(s->id, tx, "no need to store no-match sig, "
                    "mpm will revisit it");
        } else {
            TRACE_SID_TXS(s->id, tx, "storing state: flags %08x", inspect_flags);
            DetectRunStoreStateTx(scratch->sgh, f, tx->tx_ptr, tx->tx_id, s,
                    inspect_flags, flow_flags, file_no_match);
        }
    }

    return retval;
}