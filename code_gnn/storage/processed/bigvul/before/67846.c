DefragInsertFrag(ThreadVars *tv, DecodeThreadVars *dtv, DefragTracker *tracker, Packet *p, PacketQueue *pq)
{
    Packet *r = NULL;
    int ltrim = 0;

    uint8_t more_frags;
    uint16_t frag_offset;

     
    uint16_t hlen = 0;

     
    uint16_t data_offset;

     
    uint16_t data_len;

     
    uint16_t frag_end;

     
    uint16_t ip_hdr_offset;

     
    uint16_t frag_hdr_offset = 0;

     
    int af = tracker->af;

     
    int ip6_nh_set_offset = 0;
    uint8_t ip6_nh_set_value = 0;

#ifdef DEBUG
    uint64_t pcap_cnt = p->pcap_cnt;
#endif

    if (tracker->af == AF_INET) {
        more_frags = IPV4_GET_MF(p);
        frag_offset = IPV4_GET_IPOFFSET(p) << 3;
        hlen = IPV4_GET_HLEN(p);
        data_offset = (uint8_t *)p->ip4h + hlen - GET_PKT_DATA(p);
        data_len = IPV4_GET_IPLEN(p) - hlen;
        frag_end = frag_offset + data_len;
        ip_hdr_offset = (uint8_t *)p->ip4h - GET_PKT_DATA(p);

         
        if (IPV4_HEADER_LEN + frag_offset + data_len > IPV4_MAXPACKET_LEN) {
            ENGINE_SET_EVENT(p, IPV4_FRAG_PKT_TOO_LARGE);
            return NULL;
        }
    }
    else if (tracker->af == AF_INET6) {
        more_frags = IPV6_EXTHDR_GET_FH_FLAG(p);
        frag_offset = IPV6_EXTHDR_GET_FH_OFFSET(p);
        data_offset = p->ip6eh.fh_data_offset;
        data_len = p->ip6eh.fh_data_len;
        frag_end = frag_offset + data_len;
        ip_hdr_offset = (uint8_t *)p->ip6h - GET_PKT_DATA(p);
        frag_hdr_offset = p->ip6eh.fh_header_offset;

        SCLogDebug("mf %s frag_offset %u data_offset %u, data_len %u, "
                "frag_end %u, ip_hdr_offset %u, frag_hdr_offset %u",
                more_frags ? "true" : "false", frag_offset, data_offset,
                data_len, frag_end, ip_hdr_offset, frag_hdr_offset);

         
        if (ip_hdr_offset + IPV6_HEADER_LEN < frag_hdr_offset) {
            SCLogDebug("we have exthdrs before fraghdr %u bytes",
                    (uint32_t)(frag_hdr_offset - (ip_hdr_offset + IPV6_HEADER_LEN)));

             

             
            ip6_nh_set_offset = p->ip6eh.fh_prev_hdr_offset;
            ip6_nh_set_value = IPV6_EXTHDR_GET_FH_NH(p);
            SCLogDebug("offset %d, value %u", ip6_nh_set_offset, ip6_nh_set_value);
        }

         
        if (frag_offset + data_len > IPV6_MAXPACKET) {
            ENGINE_SET_EVENT(p, IPV6_FRAG_PKT_TOO_LARGE);
            return NULL;
        }
    }
    else {
         
        SCLogWarning(SC_ERR_INVALID_ARGUMENT, "Invalid address family, aborting.");
        return NULL;
    }

     
    tracker->timeout.tv_sec = p->ts.tv_sec + tracker->host_timeout;
    tracker->timeout.tv_usec = p->ts.tv_usec;

    Frag *prev = NULL, *next;
    int overlap = 0;
    if (!TAILQ_EMPTY(&tracker->frags)) {
        TAILQ_FOREACH(prev, &tracker->frags, next) {
            ltrim = 0;
            next = TAILQ_NEXT(prev, next);

            switch (tracker->policy) {
            case DEFRAG_POLICY_BSD:
                if (frag_offset < prev->offset + prev->data_len) {
                    if (frag_offset >= prev->offset) {
                        ltrim = prev->offset + prev->data_len - frag_offset;
                        overlap++;
                    }
                    if ((next != NULL) && (frag_end > next->offset)) {
                        next->ltrim = frag_end - next->offset;
                        overlap++;
                    }
                    if ((frag_offset < prev->offset) &&
                        (frag_end >= prev->offset + prev->data_len)) {
                        prev->skip = 1;
                        overlap++;
                    }
                    goto insert;
                }
                break;
            case DEFRAG_POLICY_LINUX:
                if (frag_offset < prev->offset + prev->data_len) {
                    if (frag_offset > prev->offset) {
                        ltrim = prev->offset + prev->data_len - frag_offset;
                        overlap++;
                    }
                    if ((next != NULL) && (frag_end > next->offset)) {
                        next->ltrim = frag_end - next->offset;
                        overlap++;
                    }
                    if ((frag_offset < prev->offset) &&
                        (frag_end >= prev->offset + prev->data_len)) {
                        prev->skip = 1;
                        overlap++;
                    }
                    goto insert;
                }
                break;
            case DEFRAG_POLICY_WINDOWS:
                if (frag_offset < prev->offset + prev->data_len) {
                    if (frag_offset >= prev->offset) {
                        ltrim = prev->offset + prev->data_len - frag_offset;
                        overlap++;
                    }
                    if ((frag_offset < prev->offset) &&
                        (frag_end > prev->offset + prev->data_len)) {
                        prev->skip = 1;
                        overlap++;
                    }
                    goto insert;
                }
                break;
            case DEFRAG_POLICY_SOLARIS:
                if (frag_offset < prev->offset + prev->data_len) {
                    if (frag_offset >= prev->offset) {
                        ltrim = prev->offset + prev->data_len - frag_offset;
                        overlap++;
                    }
                    if ((frag_offset < prev->offset) &&
                        (frag_end >= prev->offset + prev->data_len)) {
                        prev->skip = 1;
                        overlap++;
                    }
                    goto insert;
                }
                break;
            case DEFRAG_POLICY_FIRST:
                if ((frag_offset >= prev->offset) &&
                    (frag_end <= prev->offset + prev->data_len)) {
                    overlap++;
                    goto done;
                }
                if (frag_offset < prev->offset) {
                    goto insert;
                }
                if (frag_offset < prev->offset + prev->data_len) {
                    ltrim = prev->offset + prev->data_len - frag_offset;
                    overlap++;
                    goto insert;
                }
                break;
            case DEFRAG_POLICY_LAST:
                if (frag_offset <= prev->offset) {
                    if (frag_end > prev->offset) {
                        prev->ltrim = frag_end - prev->offset;
                        overlap++;
                    }
                    goto insert;
                }
                break;
            default:
                break;
            }
        }
    }

insert:
    if (data_len - ltrim <= 0) {
         
        goto done;
    }

     
    SCMutexLock(&defrag_context->frag_pool_lock);
    Frag *new = PoolGet(defrag_context->frag_pool);
    SCMutexUnlock(&defrag_context->frag_pool_lock);
    if (new == NULL) {
        if (af == AF_INET) {
            ENGINE_SET_EVENT(p, IPV4_FRAG_IGNORED);
        } else {
            ENGINE_SET_EVENT(p, IPV6_FRAG_IGNORED);
        }
        goto done;
    }
    new->pkt = SCMalloc(GET_PKT_LEN(p));
    if (new->pkt == NULL) {
        SCMutexLock(&defrag_context->frag_pool_lock);
        PoolReturn(defrag_context->frag_pool, new);
        SCMutexUnlock(&defrag_context->frag_pool_lock);
        if (af == AF_INET) {
            ENGINE_SET_EVENT(p, IPV4_FRAG_IGNORED);
        } else {
            ENGINE_SET_EVENT(p, IPV6_FRAG_IGNORED);
        }
        goto done;
    }
    memcpy(new->pkt, GET_PKT_DATA(p) + ltrim, GET_PKT_LEN(p) - ltrim);
    new->len = GET_PKT_LEN(p) - ltrim;
     
    if (ip6_nh_set_offset > 0 && frag_offset == 0 && ltrim == 0) {
        if (new->len > ip6_nh_set_offset) {
            SCLogDebug("updating frag to have 'correct' nh value: %u -> %u",
                    new->pkt[ip6_nh_set_offset], ip6_nh_set_value);
            new->pkt[ip6_nh_set_offset] = ip6_nh_set_value;
        }
    }

    new->hlen = hlen;
    new->offset = frag_offset + ltrim;
    new->data_offset = data_offset;
    new->data_len = data_len - ltrim;
    new->ip_hdr_offset = ip_hdr_offset;
    new->frag_hdr_offset = frag_hdr_offset;
    new->more_frags = more_frags;
#ifdef DEBUG
    new->pcap_cnt = pcap_cnt;
#endif

    Frag *frag;
    TAILQ_FOREACH(frag, &tracker->frags, next) {
        if (new->offset < frag->offset)
            break;
    }
    if (frag == NULL) {
        TAILQ_INSERT_TAIL(&tracker->frags, new, next);
    }
    else {
        TAILQ_INSERT_BEFORE(frag, new, next);
    }

    if (!more_frags) {
        tracker->seen_last = 1;
    }

    if (tracker->seen_last) {
        if (tracker->af == AF_INET) {
            r = Defrag4Reassemble(tv, tracker, p);
            if (r != NULL && tv != NULL && dtv != NULL) {
                StatsIncr(tv, dtv->counter_defrag_ipv4_reassembled);
                if (pq && DecodeIPV4(tv, dtv, r, (void *)r->ip4h,
                               IPV4_GET_IPLEN(r), pq) != TM_ECODE_OK) {

                    UNSET_TUNNEL_PKT(r);
                    r->root = NULL;
                    TmqhOutputPacketpool(tv, r);
                    r = NULL;
                } else {
                    PacketDefragPktSetupParent(p);
                }
            }
        }
        else if (tracker->af == AF_INET6) {
            r = Defrag6Reassemble(tv, tracker, p);
            if (r != NULL && tv != NULL && dtv != NULL) {
                StatsIncr(tv, dtv->counter_defrag_ipv6_reassembled);
                if (pq && DecodeIPV6(tv, dtv, r, (uint8_t *)r->ip6h,
                               IPV6_GET_PLEN(r) + IPV6_HEADER_LEN,
                               pq) != TM_ECODE_OK) {

                    UNSET_TUNNEL_PKT(r);
                    r->root = NULL;
                    TmqhOutputPacketpool(tv, r);
                    r = NULL;
                } else {
                    PacketDefragPktSetupParent(p);
                }
            }
        }
    }


done:
    if (overlap) {
        if (af == AF_INET) {
            ENGINE_SET_EVENT(p, IPV4_FRAG_OVERLAP);
        }
        else {
            ENGINE_SET_EVENT(p, IPV6_FRAG_OVERLAP);
        }
    }
    return r;
}