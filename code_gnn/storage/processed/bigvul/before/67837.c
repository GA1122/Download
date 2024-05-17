Defrag6Reassemble(ThreadVars *tv, DefragTracker *tracker, Packet *p)
{
    Packet *rp = NULL;

     
    if (!tracker->seen_last)
        return NULL;

     
    Frag *frag;
    int len = 0;
    TAILQ_FOREACH(frag, &tracker->frags, next) {
        if (frag->skip)
            continue;

        if (frag == TAILQ_FIRST(&tracker->frags)) {
            if (frag->offset != 0) {
                goto done;
            }
            len = frag->data_len;
        }
        else {
            if (frag->offset > len) {
                 
                goto done;
            }
            else {
                len += frag->data_len;
            }
        }
    }

     
    rp = PacketDefragPktSetup(p, (uint8_t *)p->ip6h,
            IPV6_GET_PLEN(p) + sizeof(IPV6Hdr), 0);
    if (rp == NULL) {
        SCLogError(SC_ERR_MEM_ALLOC, "Failed to allocate packet for "
                "fragmentation re-assembly, dumping fragments.");
        goto error_remove_tracker;
    }
    PKT_SET_SRC(rp, PKT_SRC_DEFRAG);

    int unfragmentable_len = 0;
    int fragmentable_offset = 0;
    int fragmentable_len = 0;
    int ip_hdr_offset = 0;
    uint8_t next_hdr = 0;
    TAILQ_FOREACH(frag, &tracker->frags, next) {
        if (frag->skip)
            continue;
        if (frag->data_len - frag->ltrim <= 0)
            continue;
        if (frag->offset == 0) {
            IPV6FragHdr *frag_hdr = (IPV6FragHdr *)(frag->pkt +
                frag->frag_hdr_offset);
            next_hdr = frag_hdr->ip6fh_nxt;

             
            if (PacketCopyData(rp, frag->pkt, frag->frag_hdr_offset) == -1)
                goto error_remove_tracker;
            if (PacketCopyDataOffset(rp, frag->frag_hdr_offset,
                frag->pkt + frag->frag_hdr_offset + sizeof(IPV6FragHdr),
                frag->data_len) == -1)
                goto error_remove_tracker;
            ip_hdr_offset = frag->ip_hdr_offset;

             
            fragmentable_offset = frag->frag_hdr_offset;
            fragmentable_len = frag->data_len;

             
            unfragmentable_len = (fragmentable_offset - ip_hdr_offset) - IPV6_HEADER_LEN;
            if (unfragmentable_len >= fragmentable_offset)
                goto error_remove_tracker;
        }
        else {
            if (PacketCopyDataOffset(rp, fragmentable_offset + frag->offset + frag->ltrim,
                frag->pkt + frag->data_offset + frag->ltrim,
                frag->data_len - frag->ltrim) == -1)
                goto error_remove_tracker;
            if (frag->offset + frag->data_len > fragmentable_len)
                fragmentable_len = frag->offset + frag->data_len;
        }

        if (!frag->more_frags) {
            break;
        }
    }

    rp->ip6h = (IPV6Hdr *)(GET_PKT_DATA(rp) + ip_hdr_offset);
    rp->ip6h->s_ip6_plen = htons(fragmentable_len + unfragmentable_len);
     
    if (unfragmentable_len == 0)
        rp->ip6h->s_ip6_nxt = next_hdr;
    SET_PKT_LEN(rp, ip_hdr_offset + sizeof(IPV6Hdr) +
            unfragmentable_len + fragmentable_len);

    tracker->remove = 1;
    DefragTrackerFreeFrags(tracker);
done:
    return rp;

error_remove_tracker:
    tracker->remove = 1;
    DefragTrackerFreeFrags(tracker);
    if (rp != NULL)
        PacketFreeOrRelease(rp);
    return NULL;
}
