Defrag(ThreadVars *tv, DecodeThreadVars *dtv, Packet *p, PacketQueue *pq)
{
    uint16_t frag_offset;
    uint8_t more_frags;
    DefragTracker *tracker;
    int af;

    if (PKT_IS_IPV4(p)) {
        af = AF_INET;
        more_frags = IPV4_GET_MF(p);
        frag_offset = IPV4_GET_IPOFFSET(p);
    }
    else if (PKT_IS_IPV6(p)) {
        af = AF_INET6;
        frag_offset = IPV6_EXTHDR_GET_FH_OFFSET(p);
        more_frags = IPV6_EXTHDR_GET_FH_FLAG(p);
    }
    else {
        return NULL;
    }

    if (frag_offset == 0 && more_frags == 0) {
        return NULL;
    }

    if (tv != NULL && dtv != NULL) {
        if (af == AF_INET) {
            StatsIncr(tv, dtv->counter_defrag_ipv4_fragments);
        }
        else if (af == AF_INET6) {
            StatsIncr(tv, dtv->counter_defrag_ipv6_fragments);
        }
    }

     
    tracker = DefragGetTracker(tv, dtv, p);
    if (tracker == NULL)
        return NULL;

    Packet *rp = DefragInsertFrag(tv, dtv, tracker, p, pq);
    DefragTrackerRelease(tracker);

    return rp;
}